//
// Created by mariusjenin on 24/11/22.
//

#include "EmissiveMaterial.h"
#include "TextureColor.h"

#include <utility>

using namespace texture;
using namespace component::material;

EmissiveMaterial::EmissiveMaterial(std::shared_ptr<TextureColor> albedo): Material(MaterialType::EmissiveType, std::move(albedo)) {}

Light EmissiveMaterial::generate_light() {
    auto light = Light();
    ((TextureColor*)&*m_albedo)->load_in_light_shaders(&light);
    light.set_generate_depth_map(false);
    return light;
}

color
EmissiveMaterial::resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) {
    return m_albedo->value(ray_hit->u,ray_hit->v);
}
