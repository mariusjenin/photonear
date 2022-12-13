//
// Created by mariusjenin on 19/11/22.
//

#include <utility>

#include "Material.h"
#include "DiffuseMaterial.h"
#include "Photonear.h"

using namespace component;
using namespace component::material;

std::shared_ptr<Material> Material::get_default() {
    return std::make_shared<DiffuseMaterial>();
}

ComponentType Material::get_type() {
    return typeid(this);
}

Material::Material(MaterialType type, std::shared_ptr<Texture> albedo) {
    m_type = type;
    m_albedo = std::move(albedo);
}

void Material::load_in_shaders(const std::shared_ptr<Shaders> &shaders) {
    ShadersDataManager *shader_data_manager = shaders->get_shader_data_manager();
    m_albedo->load_in_shaders(shaders);
    glUniform1f(shader_data_manager->get_location(ShadersDataManager::MATERIAL_EMISSIVE_LOC_NAME), m_type == MaterialType::EmissiveType);
}

bool Material::is_emissive() const {
    return m_type == MaterialType::EmissiveType;
}

void Material::generate_ui_component_editor() {
    m_albedo->generate_ui("Albedo");
}

void Material::attenuate(std::shared_ptr<RayCastHit> ray_hit) {
    ray_hit->attenuation *= m_albedo->value(ray_hit->u,ray_hit->v);
}

