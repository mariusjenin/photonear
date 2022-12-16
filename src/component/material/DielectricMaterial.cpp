//
// Created by mariusjenin on 08/12/22.
//

#include "DielectricMaterial.h"
#include "imgui.h"
#include "Photonear.h"

using namespace material;

DielectricMaterial::DielectricMaterial(std::shared_ptr<TextureColor> albedo, float roughness, float refractive_index) :
        DiffuseMaterial(MaterialType::RefractiveType, std::move(albedo), roughness) {
    m_refractive_index = refractive_index;
}

void DielectricMaterial::generate_ui_component_editor() {
    DiffuseMaterial::generate_ui_component_editor();

    float refractive_index = m_refractive_index;
    ImGui::SliderFloat("Refractive Index", &refractive_index, 1, 2.4);
    if (refractive_index != m_refractive_index)
        Photonear::get_instance()->get_scene()->set_scene_valid();
    m_refractive_index = refractive_index;
}

color
DielectricMaterial::resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) {
    ray_hit->attenuation *= m_albedo->value(ray_hit->u,ray_hit->v);

    if(depth == 1) {
        ray_hit->brdf = this;
        return m_albedo->value(ray_hit->u,ray_hit->v);
    }

    auto ray_dir = -ray_hit->direction;
    float dot_direction_normal = dot(ray_dir, ray_hit->normal);

    //TODO Use roughness to reflect and refract pseudorandomly

    // Schlick's approximation of Fresnel reflectance
    float n1 = ray_hit->refractive_index_of_medium;
    float n2 = m_refractive_index;
    float r0 = pow(((n2 - n1) / (n2 + n1)), 2.f);
    float fresnel_reflectance = r0 + (1.f - r0) * pow(1.f - dot_direction_normal, 5.f);

    //TODO Russian Roulette

    //Reflect
    color color_reflected;
    auto ray_reflected = Ray(ray_hit->hit_point, get_direction_reflection(ray_hit),0.0001);
    auto ray_hit_reflected = scene_graph->raycast(ray_reflected, ray_hit->refractive_index_of_medium);
    ray_hit_reflected->attenuation = ray_hit->attenuation;
    ray_hit_reflected->weight =  ray_hit->weight * fresnel_reflectance;
    if(ray_hit_reflected->hit){
        auto node = Component::get_node(ray_hit_reflected->shape);
        auto material = Component::get_nearest_component_upper<Material>(&*node);
        color_reflected = material->resolve_ray(scene_graph, ray_hit_reflected, depth - 1, default_color, false);
    } else {
        color_reflected = default_color;
    }
    ray_hit->children.emplace_back(ray_hit_reflected);

    //Refract
    color color_refracted;
    float r = n1 / n2;
    float c =dot(-1.f * ray_hit->normal, ray_dir);
    vec3 direction_refract = r * ray_dir + (r * c - sqrt(1.f - pow(r, 2.f) * (1.f - pow(c, 2.f)))) * ray_hit->normal;
    direction_refract = normalize(direction_refract);
    auto ray_refracted= Ray(ray_hit->hit_point,direction_refract,0.0001);
    auto ray_hit_refracted = scene_graph->raycast(ray_refracted, ray_hit->inner_shape?ray_hit->refractive_index_of_medium:m_refractive_index);
    ray_hit_refracted->attenuation = ray_hit->attenuation;
    ray_hit_refracted->weight =  ray_hit->weight * (1-fresnel_reflectance);
    if(ray_hit_refracted->hit){
        auto node = Component::get_node(ray_hit_refracted->shape);
        auto material = Component::get_nearest_component_upper<Material>(&*node);
        color_refracted = material->resolve_ray(scene_graph,ray_hit_refracted,depth-1, default_color, false);
    } else {
        color_refracted = default_color;
    }
    ray_hit->children.emplace_back(ray_hit_refracted);

    return (color_reflected * fresnel_reflectance + color_refracted * (1-fresnel_reflectance)) * ray_hit->attenuation;
}
