//
// Created by mariusjenin on 24/11/22.
//

#include "DiffuseMaterial.h"
#include "imgui.h"
#include "Photonear.h"

#include <utility>

using namespace component::material;

DiffuseMaterial::DiffuseMaterial(std::shared_ptr<TextureColor> albedo, float roughness) : Material(
        MaterialType::DiffuseType, std::move(albedo)) {
    m_roughness = roughness;
}

DiffuseMaterial::DiffuseMaterial(MaterialType type, std::shared_ptr<TextureColor> albedo, float roughness) :
        Material(type, std::move(albedo)) {
    m_roughness = roughness;
}

color DiffuseMaterial::resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) {
    ray_hit->attenuation *= m_albedo->value(ray_hit->u,ray_hit->v);

    if(depth == 1) {
        ray_hit->contribute = true;
        return m_albedo->value(ray_hit->u,ray_hit->v);
    }

    if(depth > 1){
        color color_bounce = {0,0,0};
        auto glossy_factor = 4.f*m_roughness*(1.f-m_roughness);
        auto diffuse_factor = m_roughness<0.5f?0.f:1.f-glossy_factor;
        auto specular_factor = m_roughness<0.5f?1.f-glossy_factor:0.f;
        if(photon_mapping_pass){
            ray_hit->contribute = true;
            float russian_roulette = (float) random() / (float) RAND_MAX;
            if (russian_roulette < glossy_factor) {
                //Glossy
                color_bounce = reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,true);//TODO change
            } else if(russian_roulette < glossy_factor+diffuse_factor){
                //Specular
                color_bounce = reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,true);
            } else {
                //Diffuse
                color_bounce = reflect(scene_graph,ray_hit,depth,get_random_direction_reflection(ray_hit),default_color,true);
            }
        } else {
            if(glossy_factor > 0) color_bounce += reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,false,glossy_factor); //TODO
            if(specular_factor > 0) color_bounce += reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,false,specular_factor);
            if(diffuse_factor > 0){
                ray_hit->contribute = true;
                ray_hit->weight *= diffuse_factor;
                color_bounce += ray_hit->attenuation * diffuse_factor;
            }
        }
        return color_bounce * ray_hit->attenuation;
    }

    return ray_hit->attenuation;
}

void DiffuseMaterial::generate_ui_component_editor() {
    Material::generate_ui_component_editor();

    float roughness = m_roughness;
    ImGui::SliderFloat("Roughness",&roughness,0,1);
    if(roughness != m_roughness)
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    m_roughness = roughness;
}

versor DiffuseMaterial::get_direction_reflection(const std::shared_ptr<RayCastHit>& ray_hit) {
    vec3 ray_dir = -ray_hit->direction;
    float dot_direction_normal = dot(ray_dir, ray_hit->normal);
    return 2.f * dot_direction_normal * ray_hit->normal - ray_dir;
}

versor DiffuseMaterial::get_random_direction_reflection(const std::shared_ptr<RayCastHit>& ray_hit) {

    float half_flt_max = ((double) RAND_MAX * 0.5f);
    versor direction;
    do{
        direction = versor((float) rand() - half_flt_max, (float) rand() - half_flt_max, (float) rand() - half_flt_max);
    } while (dot(direction,ray_hit->normal) < 0);
    direction = normalize(direction);
    std::cout << direction.x << " "<< direction.y << " "<< direction.z<< " | " << ray_hit->normal.x  << " " << ray_hit->normal.y  << " " << ray_hit->normal.z  << std::endl;
    return direction;
}

//float DiffuseMaterial::brdf(const std::shared_ptr<RayTraceHit>& ray_trace_hit, const std::shared_ptr<Photon>& photon) const {
//    return 1;
//    auto pi = (float) M_PI;
//
//    auto roughness = m_roughness;
//    auto isotropy = 1.f;
//    float n1 = 1.f;
//    float n2 = 1.f;
//    float f0 = pow(((n2 - n1) / (n2 + n1)), 2.f);
//
//    auto dir_view = ray_trace_hit->direction; //w
//    auto dir_incoming = photon->direction; //w'
//    auto n = photon->normal;
//    auto H = normalize(dir_view + dir_incoming);
//    auto u = dot(dir_view,H);
//    auto t = dot(n,H);
//    auto v = dot(dir_view,n);
//    auto vp = dot(dir_incoming,n);
//    auto T = vec3(1,1,1);
//    auto w = dot(vec3(1,1,1), H - t * n); //TODO check if correct
//    auto S = f0 + (1.f - f0) * pow(1.f - u, 5.f);
//    auto tmp = 1.f+roughness*t*t - t*t;
//    auto Zt = roughness/(tmp*tmp);
//    auto isotropy_sqr= isotropy*isotropy;
//    auto w_sqr= w*w;
//    auto Aw = isotropy/(isotropy_sqr - isotropy_sqr * w_sqr + w_sqr);
//
//    auto g = 4.f*roughness*(1.f-roughness);
//    auto d = roughness<0.5f?0.f:1.f-g;
//    auto s = roughness<0.5f?1.f-g:0.f;
//
//    auto Gv = v/(roughness - roughness*v + v);
//    auto Gvp = vp/(roughness - roughness*vp + vp);
//
//    auto D = ((Gv*Gvp*Zt*Aw)/(4.f*pi*v*vp)) + ((1.f-Gv*Gvp)/pi) * Aw;
//
//    //TODO
////    return S*(d/pi+g*D);
////std::cout << d << " " << d/pi << " " << S << std::endl;
////    return (d);
//}

color DiffuseMaterial::reflect(SceneGraph *scene_graph,const std::shared_ptr<RayCastHit>& ray_hit, int depth, versor direction,
                                  color default_color, bool photon_mapping_pass,float factor_weight){
    color color_reflect;
    auto ray_reflected = Ray(ray_hit->hit_point, get_direction_reflection(ray_hit), 0.0001);
    auto ray_hit_bounce = scene_graph->raycast(ray_reflected, ray_hit->refractive_index_of_medium);
    ray_hit->bounce_rays.push_back(ray_hit_bounce);
    ray_hit_bounce->attenuation *= ray_hit->attenuation;
    ray_hit_bounce->weight = ray_hit->weight * factor_weight;
    if (ray_hit_bounce->hit) {
        auto node = Component::get_node(ray_hit_bounce->shape);
        auto material = Component::get_nearest_component_upper<Material>(&*node);
        color_reflect = material->resolve_ray(scene_graph, ray_hit_bounce, depth - 1, default_color, photon_mapping_pass);
    } else {
        color_reflect = default_color;
    }
    return color_reflect * factor_weight;
}

