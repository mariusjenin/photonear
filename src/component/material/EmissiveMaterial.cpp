//
// Created by mariusjenin on 24/11/22.
//

#include <utility>
#include "imgui.h"

#include "EmissiveMaterial.h"
#include "TextureColor.h"
#include "Photonear.h"

using namespace texture;
using namespace component::material;

float EmissiveMaterial::DefaultIntensityForPhotonEmission = 500.f;

EmissiveMaterial::EmissiveMaterial(std::shared_ptr<TextureColor> albedo, float intensity): Material(MaterialType::EmissiveType, std::move(albedo)), m_intensity(intensity) {}

Light EmissiveMaterial::generate_light() {
    auto light = Light();
    ((TextureColor*)&*m_albedo)->load_in_light_shaders(&light);
    light.set_generate_depth_map(false);
    light.set_intensity(m_intensity);
    return light;
}

color
EmissiveMaterial::resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) {
    return m_albedo->value(ray_hit->u,ray_hit->v);
}

void EmissiveMaterial::alterate(const std::shared_ptr<RayCastHit> &ray_hit) {
    ray_hit->attenuation *= m_albedo->value(ray_hit->u,ray_hit->v);
    ray_hit->weight *= m_intensity;
}

void EmissiveMaterial::generate_ui_component_editor() {
    float intensity = m_intensity;
    Material::generate_ui_component_editor();
    ImGui::Separator();
    ImGui::DragFloat("Intensity", &intensity,0.001f, 0);

    if (m_intensity != intensity)
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    m_intensity = intensity;
}
