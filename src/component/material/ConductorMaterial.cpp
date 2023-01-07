//
// Created by mariusjenin on 08/12/22.
//

#include "ConductorMaterial.h"
#include "Shape.h"

using namespace material;

ConductorMaterial::ConductorMaterial(std::shared_ptr<TextureColor> albedo, float roughness) :
        DiffuseMaterial(MaterialType::ReflectiveType, std::move(albedo), roughness) {}

color
ConductorMaterial::resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) {
    ray_hit->attenuation *= m_albedo->value(ray_hit->u,ray_hit->v);

    if(depth == 1) {
        return DiffuseMaterial::resolve_ray(scene_graph,ray_hit,depth,default_color,photon_mapping_pass);
    }
    color reflected_color = reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,photon_mapping_pass);
    return reflected_color * ray_hit->attenuation;
}