//
// Created by mariusjenin on 24/11/22.
//

#include "EmissiveMaterial.h"
#include "TextureColor.h"

#include <utility>

using namespace texture;
using namespace component::material;

EmissiveMaterial::EmissiveMaterial(std::shared_ptr<TextureColor> albedo): Material(MaterialType::MaterialTypeEmissive, std::move(albedo)) {}

Light EmissiveMaterial::generate_light() {
    auto light = Light();
    ((TextureColor*)&*m_albedo)->load_in_light_shaders(&light);
    light.set_generate_depth_map(false);
    return light;
}

color
EmissiveMaterial::resolve_ray(SceneGraph *scene_graph, RayTraceHit ray_hit, int depth, void (*on_hit_callback)()) {
    return m_albedo->value(ray_hit.u,ray_hit.v);
}