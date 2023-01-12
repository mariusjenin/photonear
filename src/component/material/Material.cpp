//
// Created by mariusjenin on 19/11/22.
//

#include <utility>

#include "DiffuseMaterial.h"
#include "Material.h"

using namespace component;
using namespace component::material;

Material* Material::get_default() {
    return new DiffuseMaterial();
}

ComponentType Material::get_type() {
    return typeid(this);
}

Material::Material(std::shared_ptr<Texture> albedo, bool emissive) {
    m_emissive = emissive;
    m_albedo = std::move(albedo);
}

void Material::load_in_shaders(const std::shared_ptr<Shaders> &shaders) {
    ShadersDataManager *shader_data_manager = shaders->get_shader_data_manager();
    m_albedo->load_in_shaders(shaders);
    glUniform1f(shader_data_manager->get_location(ShadersDataManager::MATERIAL_EMISSIVE_LOC_NAME), m_emissive);
}

bool Material::is_emissive() const {
    return m_emissive;
}

void Material::generate_ui_component_editor() {
    m_albedo->generate_ui("Albedo");
}


versor Material::get_scattered_direction_reflection(const std::shared_ptr<RayCastHit>& ray_hit, float roughness) {
    auto perfect_reflect_dir = get_direction_reflection(ray_hit);
    auto random_reflect_dir = get_random_direction_reflection(ray_hit);
    return normalize((1.f-roughness)*perfect_reflect_dir+ roughness*random_reflect_dir);
}

versor Material::get_direction_reflection(const std::shared_ptr<RayCastHit>& ray_hit) {
    vec3 ray_dir = -ray_hit->direction;
    float dot_direction_normal = dot(ray_dir, ray_hit->normal);
    return 2.f * dot_direction_normal * ray_hit->normal - ray_dir;
}

versor Material::get_random_direction_reflection(const std::shared_ptr<RayCastHit>& ray_hit) {
    float half_flt_max = ((double) RAND_MAX * 0.5f);
    versor direction;
    do{
        direction = versor((float) rand() - half_flt_max, (float) rand() - half_flt_max, (float) rand() - half_flt_max);
    } while (dot(direction,ray_hit->normal) > 0);
    direction = normalize(direction);
    return direction;
}

color Material::reflect(SceneGraph *scene_graph, const std::shared_ptr<RayCastHit>& ray_hit, int depth, versor direction,
                               color default_color, bool photon_mapping_pass, float factor_weight){
    color color_reflect;
    auto ray_reflected = Ray(ray_hit->hit_point, direction, 0.01);
    auto ray_hit_bounce = scene_graph->raycast(ray_reflected, ray_hit->refractive_index_of_medium);
    ray_hit->bounce_rays.push_back(ray_hit_bounce);
    ray_hit_bounce->attenuation *= ray_hit->attenuation;
    ray_hit_bounce->weight = ray_hit->weight * factor_weight;
    if (ray_hit_bounce->hit) {
        auto node = Component::get_node(ray_hit_bounce->shape);
        auto material = &*Component::get_nearest_component_upper<Material>(&*node);
        if(material == nullptr) material = Material::get_default();
        color_reflect = material->resolve_ray(scene_graph, ray_hit_bounce, depth - 1, default_color, photon_mapping_pass);
    } else {
        color_reflect = default_color;
    }
    return color_reflect * factor_weight;
}

bool Material::discard_ray(const std::shared_ptr<RayCastHit>& ray_hit) {
    return ray_hit->weight <= 0.001 || length(ray_hit->attenuation) <= 0.001;
}
