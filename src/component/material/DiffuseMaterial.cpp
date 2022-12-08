//
// Created by mariusjenin on 24/11/22.
//

#include "DiffuseMaterial.h"
#include "imgui.h"
#include "Photonear.h"

#include <utility>

using namespace component::material;

DiffuseMaterial::DiffuseMaterial(std::shared_ptr<TextureColor> albedo, float roughness) : Material(
        MaterialType::MaterialTypeDiffuse, std::move(albedo)) {
    m_roughness = roughness;
}

DiffuseMaterial::DiffuseMaterial(MaterialType type, std::shared_ptr<TextureColor> albedo, float roughness) :
        Material(type, std::move(albedo)) {
    m_roughness = roughness;
}

color DiffuseMaterial::resolve_ray(SceneGraph *scene_graph, RayTraceHit ray_hit, int depth, void (*on_hit_callback)()) {
    return m_albedo->value(ray_hit.u,ray_hit.v);
}

void DiffuseMaterial::generate_ui_component_editor() {
    Material::generate_ui_component_editor();

    float roughness = m_roughness;
    ImGui::SliderFloat("Roughness",&roughness,0,1);
    if(roughness != m_roughness)
        Photonear::get_instance()->get_scene()->set_scene_valid();
    m_roughness = roughness;
}

