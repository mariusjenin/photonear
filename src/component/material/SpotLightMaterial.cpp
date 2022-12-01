//
// Created by mariusjenin on 24/11/22.
//

#include <utility>
#include "imgui.h"

#include "SpotLightMaterial.h"
#include "TransformComponent.h"
#include "Photonear.h"


using namespace component::material;

SpotLightMaterial::SpotLightMaterial(GLuint id_texture_shadow_map, std::shared_ptr<TextureColor> albedo, float in_co, float out_co, int resolution, float bias,
                                     float l_att, float q_att) : PositionnedLightMaterial(std::move(albedo),l_att,q_att) {
    m_resolution = resolution;
    m_bias = bias;
    m_inner_cutoff = in_co;
    m_outer_cutoff = out_co;
    m_inner_cutoff_computed = cos(radians(m_inner_cutoff));
    m_outer_cutoff_computed = cos(radians(m_outer_cutoff));
    m_cut_off_angle = std::fmax(m_inner_cutoff,m_outer_cutoff);
    m_id_texture_shadow_map = id_texture_shadow_map;
    m_shadow_map = std::make_shared<ShadowMap>(m_resolution,m_resolution,id_texture_shadow_map);
    m_shadow_map->activate_texture();
}

Light SpotLightMaterial::generate_light() {
    auto node = Component::get_node(this);
    auto trsf_comp = Component::get_nearest_component_upper<TransformComponent>(&*node);
    auto matrix = trsf_comp->get_matrix_as_end_node();
    Transform trsf = Transform(matrix);

    vec3 position = {0, 0, 0};
    vec3 direction = {0, 0, -1};
    vec3 up = {0, 1, 0};
    position= trsf.apply_to_point(position);
    direction = trsf.apply_to_versor(direction);
    up = trsf.apply_to_versor(up);
    mat4 view_mat = lookAt(position,position + direction,up);
    mat4 proj_mat = perspective<float>(2*radians(m_cut_off_angle),1.,1.f,100.f);

    Light light = PositionnedLightMaterial::generate_light();
    light.set_type(LIGHT_TYPE_SPOT);
    light.set_inner_cut_off(m_inner_cutoff_computed);
    light.set_outer_cut_off(m_outer_cutoff_computed);
    light.set_bias(m_bias);
    light.set_direction(direction);
    light.set_depth_vp_mat(proj_mat * view_mat);
    light.set_generate_depth_map(true);
    light.set_index_sampler_depth_map((int)m_id_texture_shadow_map);
    light.set_shadow_map(m_shadow_map);
    return light;
}

void SpotLightMaterial::generate_ui_component_editor() {
    PositionnedLightMaterial::generate_ui_component_editor();
    ImGui::Separator();
    float inner_cutoff = m_inner_cutoff;
    float outer_cutoff = m_outer_cutoff;
    float bias = m_bias;
    int resolution = m_resolution;
    ImGui::SliderFloat("Outer Cutoff",&outer_cutoff,0,90);
    ImGui::SliderFloat("Inner Cutoff",&inner_cutoff,0,m_outer_cutoff);
    ImGui::SliderFloat("Bias Shadow Map",&bias,0,1,"%.3f",ImGuiSliderFlags_Logarithmic);
    ImGui::SliderInt("Resolution Shadow Map",&resolution,2,4000);

    auto inner_cutoff_changed = inner_cutoff != m_inner_cutoff || inner_cutoff > outer_cutoff;
    auto outer_cutoff_changed = outer_cutoff != m_outer_cutoff;

    if(inner_cutoff_changed) m_inner_cutoff = inner_cutoff;
    if(outer_cutoff_changed) m_outer_cutoff = outer_cutoff;
    if(m_inner_cutoff > m_outer_cutoff) m_inner_cutoff = m_outer_cutoff;
    if(inner_cutoff_changed) m_inner_cutoff_computed = cos(radians(m_inner_cutoff));
    if(outer_cutoff_changed) m_outer_cutoff_computed = cos(radians(m_outer_cutoff));
    if(inner_cutoff_changed || outer_cutoff_changed) m_cut_off_angle = std::fmax(m_inner_cutoff,m_outer_cutoff);

    if(inner_cutoff_changed || outer_cutoff_changed || m_bias != bias || m_resolution != resolution)
        Photonear::get_instance()->get_scene()->set_scene_valid();

    m_bias = bias;
    m_resolution = resolution;

}
