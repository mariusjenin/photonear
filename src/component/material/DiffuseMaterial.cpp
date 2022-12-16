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
    ray_hit->brdf = this;
    if(photon_mapping_pass && depth > 1){
        auto ray_reflected = Ray(ray_hit->hit_point,get_direction_reflection(ray_hit),0.0001);
        ray_hit->bounce_ray = scene_graph->raycast(ray_reflected, ray_hit->refractive_index_of_medium);
        ray_hit->bounce_ray->attenuation = ray_hit->attenuation;
        ray_hit->bounce_ray->weight = ray_hit->weight;
        if(ray_hit->bounce_ray->hit){
            auto node = Component::get_node(ray_hit->bounce_ray->shape);
            auto material = Component::get_nearest_component_upper<Material>(&*node);
            material->resolve_ray(scene_graph,ray_hit->bounce_ray,depth-1, default_color, true);
        }
    }
    ray_hit->brdf = this;
    return ray_hit->attenuation;
}

void DiffuseMaterial::generate_ui_component_editor() {
    Material::generate_ui_component_editor();

    float roughness = m_roughness;
    ImGui::SliderFloat("Roughness",&roughness,0,1);
    if(roughness != m_roughness)
        Photonear::get_instance()->get_scene()->set_scene_valid();
    m_roughness = roughness;
}

versor DiffuseMaterial::get_direction_reflection(const std::shared_ptr<RayCastHit>& ray_hit) {

    //TODO Use roughness to reflect pseudorandomly

    vec3 ray_dir = -ray_hit->direction;
    float dot_direction_normal = dot(ray_dir, ray_hit->normal);
    return 2.f * dot_direction_normal * ray_hit->normal - ray_dir;
}

float DiffuseMaterial::oren_nayar_brdf(const std::shared_ptr<RayTraceHit>& ray_trace_hit,const std::shared_ptr<Photon>& photon) const {
    auto l = photon->direction;
    auto v = ray_trace_hit->direction;
    auto n = photon->normal;

    float nl = dot(n, l);
    float nv = dot(n, v);

    float theta_i = acos(nl);
    float theta_r = acos(nv);

    float sigma_sqr = m_roughness * m_roughness;
    float A = 1 - 0.5f * (sigma_sqr / (sigma_sqr + 0.33f));
    float B = 0.45f * (sigma_sqr / (sigma_sqr + 0.09f));
    float alpha = max(theta_i,theta_r);
    float beta = min(theta_i,theta_r);

    return (A + B * std::max(0.0f, cos(theta_i - theta_r)) * sin(alpha) * tan(beta));
}

