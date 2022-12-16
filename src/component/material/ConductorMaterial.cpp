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
        ray_hit->brdf = this;
        return m_albedo->value(ray_hit->u,ray_hit->v);
    }

    color reflected_color;
    auto ray_reflected = Ray(ray_hit->hit_point,get_direction_reflection(ray_hit),0.001);
    ray_hit->bounce_ray = scene_graph->raycast(ray_reflected, ray_hit->refractive_index_of_medium);
    ray_hit->bounce_ray->attenuation = ray_hit->attenuation;
    ray_hit->bounce_ray->weight = ray_hit->weight;
    if(ray_hit->bounce_ray->hit){
        auto node = Component::get_node(ray_hit->bounce_ray->shape);
        auto material = Component::get_nearest_component_upper<Material>(&*node);
        reflected_color = material->resolve_ray(scene_graph,ray_hit->bounce_ray,depth-1, default_color,false);
    } else {
        reflected_color = default_color;
    }
    return reflected_color * ray_hit->attenuation;
}