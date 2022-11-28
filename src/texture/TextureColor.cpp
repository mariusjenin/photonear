//
// Created by mariusjenin on 22/11/22.
//

#include "imgui.h"

#include "TextureColor.h"
#include "Material.h"

using namespace texture;
using namespace component;
using namespace component::material;

color TextureColor::value(float u, float v, point p) const {
    return m_value;
}

TextureColor::TextureColor() {
    m_value = color();
}

TextureColor::TextureColor(color c) {
    m_value = c;
}

TextureColor::TextureColor(float r, float g, float b) {
    m_value = {r,g,b};
}

void TextureColor::load_in_shaders(const std::shared_ptr<Shaders> &shaders) {
    ShadersDataManager *shader_data_manager = shaders->get_shader_data_manager();
    glUniform3fv(shader_data_manager->get_location(ShadersDataManager::MATERIAL_ALBEDO_COLOR_LOC_NAME), 1, &m_value[0]);
    glUniform1f(shader_data_manager->get_location(ShadersDataManager::MATERIAL_TYPE_LOC_NAME), Material::MATERIAL_TYPE_COLOR);
}

void TextureColor::load_in_light_shaders(Light* light) {
    light->set_albedo(m_value);
}

TextureColor::TextureColor(float v) {
    m_value = {v,v,v};
}

void TextureColor::generate_ui(const std::string& name) {
    ImGui::ColorEdit3(name.c_str(),&m_value[0]);
}
