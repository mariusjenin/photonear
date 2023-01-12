//
// Created by mariusjenin on 12/01/23.
//

#include "DiffuseMaterial.h"
#include "imgui.h"
#include "Photonear.h"

#include <utility>
#define stringify( name ) #name

using namespace component::material;

DiffuseMaterial::DiffuseMaterial(DiffuseMaterialType type, std::shared_ptr<TextureColor> albedo, float roughness) : Material(
        std::move(albedo),false) {
    m_type = type;
    m_roughness = roughness;
}

color DiffuseMaterial::resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) {
    ray_hit->attenuation *= m_albedo->value(ray_hit->u,ray_hit->v);

    if(depth == 1 || discard_ray(ray_hit)) {
        ray_hit->contribute = true;
        return m_albedo->value(ray_hit->u,ray_hit->v);
    }

    if(depth > 1) {
        color color_bounce;
        switch (m_type) {
            case Plastic:
                color_bounce = plastic_resolve_ray(scene_graph, ray_hit, depth, default_color,
                                    photon_mapping_pass);
                break;
            case Metal:
                color_bounce = metal_resolve_ray(scene_graph, ray_hit, depth, default_color,
                                  photon_mapping_pass);
                break;
        }
        return color_bounce * ray_hit->attenuation;
    }

    return ray_hit->attenuation;
}

void DiffuseMaterial::generate_ui_component_editor() {
    Material::generate_ui_component_editor();
    std::map<DiffuseMaterialType,std::string> types = {{Plastic,stringify(Plastic)},{Metal,stringify(Metal)}};

    float roughness = m_roughness;
    DiffuseMaterialType type = m_type;

    if (ImGui::BeginCombo("Type", types.at(type).c_str()))
    {
        for (const auto& item : types)
        {
            if (ImGui::Selectable(item.second.c_str(), (type == item.first)))
                type = item.first;
        }
        ImGui::EndCombo();
    }

    ImGui::SliderFloat("Roughness",&roughness,0,1);
    if(roughness != m_roughness || type != m_type)
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    m_roughness = roughness;
    m_type = type;
}

color DiffuseMaterial::plastic_resolve_ray(SceneGraph *scene_graph, const std::shared_ptr<RayCastHit>& ray_hit, int depth,
                                           color default_color,
                                           bool photon_mapping_pass) const {
    color color_bounce = {0,0,0};
    if(photon_mapping_pass){
        ray_hit->contribute = true;
        float russian_roulette = (float) random() / (float) RAND_MAX;
        versor direction;
        if(russian_roulette < m_roughness){
            //Diffuse
            direction = get_random_direction_reflection(ray_hit);
        } else {
            //Specular
            direction = get_direction_reflection(ray_hit);
        }
        color_bounce = reflect(scene_graph,ray_hit,depth,direction,default_color,true);
    } else {
        if(1-m_roughness > 0){
            color_bounce += reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,false,1-m_roughness);
        }
        if(m_roughness > 0){
            ray_hit->contribute = true;
            ray_hit->weight *= m_roughness;
            color_bounce += ray_hit->attenuation * m_roughness;
        }
    }
    return color_bounce * ray_hit->attenuation;
}

color DiffuseMaterial::metal_resolve_ray(SceneGraph *scene_graph, const std::shared_ptr<RayCastHit>& ray_hit, int depth,
                                         color default_color, bool photon_mapping_pass) const {
        color color_bounce = {0,0,0};
        if(photon_mapping_pass){
            ray_hit->contribute = true;
            versor direction;
            if (m_roughness == 1) {
                //Diffuse
                direction = get_random_direction_reflection(ray_hit);
            } else if (m_roughness == 0) {
                //Specular
                direction = get_direction_reflection(ray_hit);
            } else {
                //Scattered
                direction = get_scattered_direction_reflection(ray_hit,m_roughness);
            }
            color_bounce = reflect(scene_graph,ray_hit,depth,direction,default_color,true);
        } else {
            if(m_roughness == 1){
                //Diffuse
                ray_hit->contribute = true;
                ray_hit->weight *= m_roughness;
                color_bounce = ray_hit->attenuation * m_roughness;
            }else if (m_roughness == 0) {
                //Specular
                color_bounce = reflect(scene_graph,ray_hit,depth,get_direction_reflection(ray_hit),default_color,false);
            } else {
                //Scattered
                color_bounce = reflect(scene_graph, ray_hit, depth, get_scattered_direction_reflection(ray_hit,m_roughness), default_color, false);
            }

        }
        return color_bounce * ray_hit->attenuation;
}

