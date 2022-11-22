#version 330 core
//VERTEX DATA
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

//MVP
uniform mat4 model_mat;
uniform mat4 normal_model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

//OUT
out vec2 uv;
out vec3 normal;
out vec3 fragment_pos;

uniform bool debug_rendering;

void main(){
    vec4 vertex_world_position;
    mat4 vp = projection_mat * view_mat ;
    if(debug_rendering){
        vertex_world_position = vec4(vertex_position,1);
    } else {
        uv = vertex_uv;
        normal = normalize(vec3(normal_model_mat*vec4(vertex_normal,0)));


        vertex_world_position = model_mat * vec4(vertex_position,1);

        fragment_pos = vec3(vertex_world_position);
    }
    gl_Position = vp* vertex_world_position;
}

