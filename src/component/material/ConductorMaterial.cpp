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
        ray_hit->diffuse = true;
        return m_albedo->value(ray_hit->u,ray_hit->v);
    }

    vec3 ray_dir = -ray_hit->direction;
    vec3 halfway_vector = (ray_hit->normal + ray_dir) / 2.f;
    float dot_direction_normal = dot(ray_dir, ray_hit->normal);

    //TODO Use roughness to reflect pseudorandomly

    color reflected_color;
    auto ray_reflected = Ray(ray_hit->hit_point,2.f * dot_direction_normal * ray_hit->normal - ray_dir,0.0001);
    auto ray_hit_reflection = scene_graph->raycast(ray_reflected, ray_hit->refractive_index_of_medium);
    ray_hit_reflection->attenuation = ray_hit->attenuation;
    ray_hit_reflection->weight = ray_hit->weight;
    ray_hit_reflection->diffuse = false;
    if(ray_hit_reflection->hit){
        auto node = Component::get_node(ray_hit_reflection->shape);
        auto material = Component::get_nearest_component_upper<Material>(&*node);
        reflected_color = material->resolve_ray(scene_graph,ray_hit_reflection,depth-1, default_color,false);
    } else {
        reflected_color = default_color;
    }
    ray_hit->children.emplace_back(ray_hit_reflection);
    return reflected_color * ray_hit->attenuation;
}