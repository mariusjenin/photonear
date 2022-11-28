//
// Created by mariusjenin on 24/11/22.
//

#include "imgui.h"
#include <utility>

#include "PositionnedLightMaterial.h"
#include "TransformComponent.h"


using namespace component;
using namespace component::material;

PositionnedLightMaterial::PositionnedLightMaterial(std::shared_ptr<TextureColor> albedo, float l_att, float q_att) : LightMaterial(std::move(albedo)){
    m_linear_attenuation = l_att;
    m_quadratic_attenuation = q_att;
}

Light PositionnedLightMaterial::generate_light() {
    auto node = Component::get_node(this);
    auto trsf_comp = Component::get_nearest_component_upper<TransformComponent>(&*node);
    auto matrix = trsf_comp->get_matrix_as_end_node();
    Transform trsf = Transform(matrix);
    vec3 position = {};
    position = trsf.apply_to_point(position);

    Light light = LightMaterial::generate_light();
    light.set_type(LIGHT_TYPE_POINT);
    light.set_linear_attenuation(m_linear_attenuation);
    light.set_quadratic_attenuation(m_quadratic_attenuation);
    light.set_position(position);
    return light;
}

void PositionnedLightMaterial::generate_component_editor_ui() {
    Material::generate_component_editor_ui();
    ImGui::Separator();
    ImGui::SliderFloat("Linear Attenuation",&m_linear_attenuation,0,1,"%.3f", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Quadratic Attenuation",&m_quadratic_attenuation,0,1,"%.3f", ImGuiSliderFlags_Logarithmic);
}
