//
// Created by mariusjenin on 24/11/22.
//

#include "imgui.h"
#include <utility>

#include "PositionnedEmissiveMaterial.h"
#include "TransformComponent.h"
#include "Photonear.h"


using namespace component;
using namespace component::material;

PositionnedEmissiveMaterial::PositionnedEmissiveMaterial(std::shared_ptr<TextureColor> albedo, float intensity,
                                                         float l_att, float q_att) :
        EmissiveMaterial(std::move(albedo), intensity), m_linear_attenuation(l_att), m_quadratic_attenuation(q_att) {}

Light PositionnedEmissiveMaterial::generate_light() {
    auto node = Component::get_node(this);
    auto trsf_comp = Component::get_nearest_component_upper<TransformComponent>(&*node);
    auto matrix = trsf_comp->get_matrix_as_end_node();
    Transform trsf = Transform(matrix);
    vec3 position = {0, 0, 0};
    position = trsf.apply_to_point(position);

    Light light = EmissiveMaterial::generate_light();
    light.set_type(LightType::PointType);
    light.set_linear_attenuation(m_linear_attenuation);
    light.set_quadratic_attenuation(m_quadratic_attenuation);
    light.set_position(position);
    return light;
}

void PositionnedEmissiveMaterial::generate_ui_component_editor() {
    float linear_attenuation = m_linear_attenuation;
    float quadratic_attenuation = m_quadratic_attenuation;
    EmissiveMaterial::generate_ui_component_editor();
    ImGui::Separator();
    ImGui::SliderFloat("Linear Attenuation", &linear_attenuation, 0, 1, "%.3f", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Quadratic Attenuation", &quadratic_attenuation, 0, 1, "%.3f", ImGuiSliderFlags_Logarithmic);

    if (m_linear_attenuation != linear_attenuation || m_quadratic_attenuation != quadratic_attenuation)
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    m_linear_attenuation = linear_attenuation;
    m_quadratic_attenuation = quadratic_attenuation;
}

Ray PositionnedEmissiveMaterial::get_random_ray(glm::mat4 matrix) {
    float half_flt_max = ((double) RAND_MAX * 0.5f);
    versor direction =
            normalize(versor((float) rand() - half_flt_max, (float) rand() - half_flt_max, (float) rand() - half_flt_max));
    direction = normalize(vec3(matrix * vec4(direction, 0)));
    point origin = vec3(matrix * vec4(vec3(0, 0, 0), 1));
    return {origin, direction, 0.0001};
}