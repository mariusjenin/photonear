//
// Created by mariusjenin on 08/12/22.
//

#include "DielectricMaterial.h"
#include "imgui.h"
#include "Photonear.h"

using namespace material;

DielectricMaterial::DielectricMaterial(std::shared_ptr<TextureColor> albedo, float roughness, float refractive_index) :
        DiffuseMaterial(MaterialType::MaterialTypeRefractive, std::move(albedo), roughness) {
    m_refractive_index = refractive_index;
}

void DielectricMaterial::generate_ui_component_editor() {
    DiffuseMaterial::generate_ui_component_editor();

    float refractive_index = m_refractive_index;
    ImGui::SliderFloat("Refractive Index",&refractive_index,1,2.4);
    if(refractive_index != m_refractive_index)
        Photonear::get_instance()->get_scene()->set_scene_valid();
    m_refractive_index = refractive_index;
}

void DielectricMaterial::load_in_shaders(const std::shared_ptr<Shaders> &shaders) {
    ShadersDataManager *shader_data_manager = shaders->get_shader_data_manager();
    Material::load_in_shaders(shaders);
    float tmp = 1 - (m_refractive_index-1.f) / 1.4f;
    float transparency = (tmp * tmp * tmp * 0.9f) + 0.1f;
    glUniform1f(shader_data_manager->get_location(ShadersDataManager::MATERIAL_TRANSPARENCY_LOC_NAME),transparency);
}
