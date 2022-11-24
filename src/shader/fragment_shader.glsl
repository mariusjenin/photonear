#version 430 core
#define NB_MAX_LIGHTS 10

struct Material{
    int type;
    float shininess;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //With texture
    sampler2D diffuse_texture;
    sampler2D specular_texture;
};

struct Light {
    int type;
    vec3 ambient;
    float pad0;
    vec3 diffuse;
    float pad1;
    vec3 specular;
    float pad2;
    //Positionned Light
    vec3  position;
    float pad3;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
    //Directed Light
    vec3  direction;
    float pad4;
    //SpotLight
    float inner_cut_off;
    float outer_cut_off; // if inner == outer then no smooth edge
    //Depth and Shadow Map
    int generate_depth_map;
    int index_shadow_map;
//    float bias_depth_map;
    mat4 depth_vp_mat;
};

//CONSTANT
uniform int LIGHT_TYPE_DIRECTIONAL;
uniform int LIGHT_TYPE_POINT;
uniform int LIGHT_TYPE_SPOT;

uniform int MATERIAL_TYPE_COLOR;
uniform int MATERIAL_TYPE_TEXTURE;

//IN
in vec2 uv;
in vec3 normal;
in vec3 fragment_pos;

//Lights
layout(std430, binding = 3) buffer lights_buffer
        {
                Light lights_from_buffer[];
        };
uniform int nb_lights;
uniform sampler2D shadow_maps[NB_MAX_LIGHTS];

//CAMERA_POS
uniform vec3 view_pos;

//MATRIX
uniform mat4 model_mat;

//DATA Object
uniform Material material;
uniform bool debug_rendering;
uniform vec3 debug_rendering_color;

out vec3 color;


uniform bool is_node_on_top;
vec3 compute_phong(Light light, sampler2D shadow_map){
    //AMBIENT
    vec3 ambient = vec3(0,0,0);
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);

    if(material.type == MATERIAL_TYPE_COLOR) {
        ambient = light.ambient *  material.ambient;
    } else if(material.type == MATERIAL_TYPE_TEXTURE) {
        ambient = light.ambient * texture(material.diffuse_texture, uv).rgb;
    }


    //DIFFUSE
    vec3 light_dir;
    if(light.type == LIGHT_TYPE_DIRECTIONAL){
        light_dir = normalize(-light.direction);
    } else if(light.type == LIGHT_TYPE_POINT || light.type == LIGHT_TYPE_SPOT){
        light_dir = normalize(light.position - fragment_pos);
    }

    float diff = max(dot(normal, light_dir), 0.0);
    if(material.type == MATERIAL_TYPE_COLOR) {
        diffuse = light.diffuse * diff * material.diffuse;
    } else  if(material.type == MATERIAL_TYPE_TEXTURE) {
        diffuse = light.diffuse * diff * texture(material.diffuse_texture, uv).rgb;
    }

    //SPECULAR
    if(material.shininess > 0){
        vec3 view_dir = normalize(view_pos - fragment_pos);
        vec3 reflect_dir = reflect(-light_dir, normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        if(material.type == MATERIAL_TYPE_COLOR) {
            specular = light.specular * spec * material.specular;
        } else  if(material.type == MATERIAL_TYPE_TEXTURE) {
            specular = light.specular * spec * texture(material.specular_texture, uv).rgb;
        }
    }

    //CUTOFF (with soft edges)
    if(light.type == LIGHT_TYPE_SPOT){
        float theta = dot(light_dir, normalize(-light.direction));
        float epsilon = (light.inner_cut_off - light.outer_cut_off);
        float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
    }

    //ATTENUATION
    if(( light.type == LIGHT_TYPE_POINT || light.type == LIGHT_TYPE_SPOT ) && (light.constant_attenuation != 0 || light.linear_attenuation != 0 || light.quadratic_attenuation != 0) ){
        float distance    = length(light.position - fragment_pos);
        float attenuation = 1.0 / (light.constant_attenuation + light.linear_attenuation * distance + light.quadratic_attenuation * (distance * distance));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    //SHADOW MAPS
    float shadow = 0.0;
    if(light.generate_depth_map == 1) {
        vec4 frag_pos_light_space = light.depth_vp_mat * vec4(fragment_pos,1.0);
        //perform perspective divide
        vec3 proj_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
        //transform to [0,1] range
        proj_coords = proj_coords * 0.5 + 0.5;
        if(proj_coords.z <= 1.0){
            //get depth of current fragment from light's perspective
            float current_depth = proj_coords.z;
            //PCF
            int kernel_size = 3; //odd
            int kernel_bounds = (kernel_size-1)/2;
            vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
            //BIAS
//            float bias = max(light.bias_depth_map * (1.0 - dot(normal, light_dir)), light.bias_depth_map);
            float bias = max(0.004 * (1.0 - dot(normal, light_dir)), 0.0004);
            for(int x = -kernel_bounds; x <= kernel_bounds; ++x)
            {
                for(int y = -kernel_bounds; y <= kernel_bounds; ++y)
                {
                    float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
                    shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
                }
            }
            shadow /= kernel_size*kernel_size;
        }
    }

    return ambient + (1.0 - shadow) * (diffuse + specular);
}

void main() {
    if(debug_rendering){
        color = debug_rendering_color;
    } else {
        color = vec3(0,0,0);
        color = normal;
        for(int i = 0 ; i < nb_lights ; i++) {
            int shadow_map_index = int(lights_from_buffer[i].index_shadow_map);
            color += compute_phong(lights_from_buffer[i], shadow_maps[shadow_map_index]);
        }
    }
}