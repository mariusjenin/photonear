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
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    m_refractive_index = refractive_index;
}

color
DielectricMaterial::resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth,
                                color default_color, bool photon_mapping_pass) {
    if (depth == 1) {
        return DiffuseMaterial::resolve_ray(scene_graph,ray_hit,depth,default_color,photon_mapping_pass);
    }

    ray_hit->attenuation *= m_albedo->value(ray_hit->u, ray_hit->v);

    auto ray_dir = -ray_hit->direction;
    float dot_direction_normal = dot(ray_dir, ray_hit->normal);

    // Schlick's approximation of Fresnel reflectance
    float n1 = ray_hit->refractive_index_of_medium;
    float n2 = m_refractive_index;
    float f0 = pow(((n2 - n1) / (n2 + n1)), 2.f);
    float fresnel_reflectance = f0 + (1.f - f0) * pow(1.f - dot_direction_normal, 5.f);

    color color_bounce = {0,0,0};

    if(photon_mapping_pass){
        float russian_roulette = (float) random() / (float) RAND_MAX;
        if (russian_roulette < fresnel_reflectance) {
            //Reflect
            color_bounce = reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,true);
        } else {
            //Refract
            color_bounce = refract(scene_graph,ray_hit,depth,default_color,true);
        }
    } else {
        //Reflect
        color_bounce += reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,false,fresnel_reflectance);
        //Refract
        color_bounce += refract(scene_graph,ray_hit,depth,default_color,false,1-fresnel_reflectance);
    }

    return color_bounce * ray_hit->attenuation;
}

color DielectricMaterial::refract(SceneGraph *scene_graph,const std::shared_ptr<RayCastHit>& ray_hit, int depth,
                               color default_color, bool photon_mapping_pass,float factor_weight){
    auto ray_dir = -ray_hit->direction;
    float n1 = ray_hit->refractive_index_of_medium;
    float n2 = m_refractive_index;
    color color_refract;
    float r = n1 / n2;
    vec3 direction_refract = -r * (ray_dir - dot(ray_dir,ray_hit->normal)*ray_hit->normal) -
                             sqrt(1-r*r*(1-(dot(ray_dir,ray_hit->normal)*dot(ray_dir,ray_hit->normal)))) *ray_hit->normal ;
    direction_refract = normalize(direction_refract);
    auto ray_refracted = Ray(ray_hit->hit_point, direction_refract, 0.0001);

    auto ray_cast_hit = scene_graph->raycast(ray_refracted, ray_hit->inner_shape ? ray_hit->refractive_index_of_medium : m_refractive_index);
    ray_hit->bounce_rays.push_back(ray_cast_hit);
    ray_cast_hit->attenuation = ray_hit->attenuation;
    ray_cast_hit->weight = ray_hit->weight * factor_weight;
    if (ray_cast_hit->hit) {
        auto node = Component::get_node(ray_cast_hit->shape);
        auto material = Component::get_nearest_component_upper<Material>(&*node);
        color_refract = material->resolve_ray(scene_graph, ray_cast_hit, depth - 1, default_color, photon_mapping_pass);
    } else {
        color_refract = default_color;
    }
    return color_refract * factor_weight;
}

