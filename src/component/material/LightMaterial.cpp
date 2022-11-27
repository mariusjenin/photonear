//
// Created by mariusjenin on 24/11/22.
//

#include "LightMaterial.h"
#include "TextureColor.h"

#include <utility>

using namespace texture;
using namespace component::material;

LightMaterial::LightMaterial(std::shared_ptr<TextureColor> albedo): Material(std::move(albedo)) {
    m_emissive = true;
    m_refractive = false;
    m_metallic = false;
}

Light LightMaterial::generate_light() {
    auto light = Light();
    ((TextureColor*)&*m_albedo)->load_in_light_shaders(&light);
    light.set_generate_depth_map(false);
    return light;
}
