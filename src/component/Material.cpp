//
// Created by mariusjenin on 19/11/22.
//

#include "Material.h"

#include <utility>

using namespace component;

std::shared_ptr<Material> Material::get_default() {
    return std::make_shared<Material>();
}

ComponentType Material::get_type() {
    return typeid(this);
}

Material::Material(std::shared_ptr<Texture> albedo, bool emissive, bool metallic, float metallic_fuzz, bool refractive,
                   float refractive_index) {
    m_albedo = std::move(albedo);
    m_emissive = emissive;
    m_metallic = metallic;
    m_metallic_fuzz = metallic_fuzz;
    m_refractive = refractive;
    m_refractive_index = refractive_index;
}

