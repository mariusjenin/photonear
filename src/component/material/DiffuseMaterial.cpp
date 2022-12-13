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
    ray_hit->diffuse = true;
    if(photon_mapping_pass && depth > 1){
        vec3 ray_dir = -ray_hit->direction;
        vec3 halfway_vector = (ray_hit->normal + ray_dir) / 2.f;
        float dot_direction_normal = dot(ray_dir, ray_hit->normal);

        //TODO Use roughness to reflect pseudorandomly

        auto ray_reflected = Ray(ray_hit->hit_point,2.f * dot_direction_normal * ray_hit->normal - ray_dir,0.0001);
        auto ray_hit_reflection = scene_graph->raycast(ray_reflected, ray_hit->refractive_index_of_medium);
        ray_hit_reflection->attenuation = ray_hit->attenuation;
        ray_hit_reflection->weight = ray_hit->weight;
        ray_hit->diffuse = false;
        if(ray_hit_reflection->hit){
            auto node = Component::get_node(ray_hit_reflection->shape);
            auto material = Component::get_nearest_component_upper<Material>(&*node);
            material->resolve_ray(scene_graph,ray_hit_reflection,depth-1, default_color, true);
        }
        ray_hit->children.emplace_back(ray_hit_reflection);
    }
    ray_hit->diffuse = true;
    return ray_hit->attenuation * m_albedo->value(ray_hit->u,ray_hit->v);
}

void DiffuseMaterial::generate_ui_component_editor() {
    Material::generate_ui_component_editor();

    float roughness = m_roughness;
    ImGui::SliderFloat("Roughness",&roughness,0,1);
    if(roughness != m_roughness)
        Photonear::get_instance()->get_scene()->set_scene_valid();
    m_roughness = roughness;
}

