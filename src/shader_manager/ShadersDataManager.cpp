//
// Created by mariusjenin on 21/11/22.
//

#include "ShadersDataManager.h"
#include "LightMaterial.h"

using namespace shader_manager;
using namespace component::material;

GLint ShadersDataManager::get_location(const std::string &name) {
    return m_locations[name];
}

void ShadersDataManager::load_matrices_locations(GLuint program_id) {
    m_locations[MODEL_MAT_LOC_NAME] = glGetUniformLocation(program_id, MODEL_MAT_LOC_NAME);
    m_locations[NORMAL_MODEL_MAT_LOC_NAME] = glGetUniformLocation(program_id, NORMAL_MODEL_MAT_LOC_NAME);
    m_locations[VIEW_MAT_LOC_NAME] = glGetUniformLocation(program_id, VIEW_MAT_LOC_NAME);
    m_locations[PROJ_MAT_LOC_NAME] = glGetUniformLocation(program_id, PROJ_MAT_LOC_NAME);
}

void ShadersDataManager::load_view_pos_location(GLuint program_id) {
    m_locations[VIEW_POS_LOC_NAME] = glGetUniformLocation(program_id, VIEW_POS_LOC_NAME);
}

void ShadersDataManager::load_lights_locations(GLuint program_id) {
    m_locations[NB_LIGTHS_LOC_NAME] = glGetUniformLocation(program_id, NB_LIGTHS_LOC_NAME);
    m_locations[BLOCK_INDEX_LIGHTS_LOC_NAME] = glGetProgramResourceIndex(program_id, GL_SHADER_STORAGE_BLOCK,
                                                                         BLOCK_INDEX_LIGHTS_LOC_NAME);
}

void ShadersDataManager::load_material_const(GLuint program_id) {
    m_locations[MATERIAL_TYPE_COLOR_LOC_NAME] = glGetUniformLocation(program_id, MATERIAL_TYPE_COLOR_LOC_NAME);
    m_locations[MATERIAL_TYPE_TEXTURE_LOC_NAME] = glGetUniformLocation(program_id, MATERIAL_TYPE_TEXTURE_LOC_NAME);
    glUniform1i(m_locations[MATERIAL_TYPE_COLOR_LOC_NAME], Material::MATERIAL_TYPE_COLOR);
    glUniform1i(m_locations[MATERIAL_TYPE_TEXTURE_LOC_NAME], Material::MATERIAL_TYPE_TEXTURE);
}

void ShadersDataManager::load_material_locations(GLuint program_id) {
    m_locations[MATERIAL_TYPE_LOC_NAME] = glGetUniformLocation(program_id, MATERIAL_TYPE_LOC_NAME);
    m_locations[MATERIAL_ALBEDO_COLOR_LOC_NAME] = glGetUniformLocation(program_id, MATERIAL_ALBEDO_COLOR_LOC_NAME);
    m_locations[MATERIAL_ALBEDO_TEXTURE_LOC_NAME] = glGetUniformLocation(program_id, MATERIAL_ALBEDO_TEXTURE_LOC_NAME);
    m_locations[MATERIAL_EMISSIVE_LOC_NAME] = glGetUniformLocation(program_id, MATERIAL_EMISSIVE_LOC_NAME);
}

void ShadersDataManager::load_lights_const(GLuint program_id) {
    m_locations[LIGHT_TYPE_DIRECTIONAL_LOC_NAME] = glGetUniformLocation(program_id, LIGHT_TYPE_DIRECTIONAL_LOC_NAME);
    m_locations[LIGHT_TYPE_POINT_LOC_NAME] = glGetUniformLocation(program_id, LIGHT_TYPE_POINT_LOC_NAME);
    m_locations[LIGHT_TYPE_SPOT_LOC_NAME] = glGetUniformLocation(program_id, LIGHT_TYPE_SPOT_LOC_NAME);
    glUniform1i(m_locations[LIGHT_TYPE_DIRECTIONAL_LOC_NAME], LightMaterial::LIGHT_TYPE_DIRECTIONAL);
    glUniform1i(m_locations[LIGHT_TYPE_POINT_LOC_NAME], LightMaterial::LIGHT_TYPE_POINT);
    glUniform1i(m_locations[LIGHT_TYPE_SPOT_LOC_NAME], LightMaterial::LIGHT_TYPE_SPOT);
}

void ShadersDataManager::load_custom_uniform_location(GLuint program_id, const std::string &name) {
    m_locations[name] = glGetUniformLocation(program_id, name.c_str());
}

void ShadersDataManager::load_lights(GLuint program_id, LightShader lights_shader[], int size_lights) {

    //NB LIGHTS
    GLint nb_lights_location = get_location(ShadersDataManager::NB_LIGTHS_LOC_NAME);
    glUniform1i(nb_lights_location, (int) size_lights);

    //LIGHTS BUFFER
    GLuint block_index = get_location(ShadersDataManager::BLOCK_INDEX_LIGHTS_LOC_NAME);
    GLuint ssbo_binding_point_index = 3;
    glShaderStorageBlockBinding(program_id, block_index, ssbo_binding_point_index);

    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (int) sizeof(LightShader) * size_lights, &lights_shader[0], GL_STATIC_READ);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, ssbo);
}

void ShadersDataManager::load_debug_const(GLuint program_id) {
    m_locations[DEBUG_RENDERING_LOC_NAME] = glGetUniformLocation(program_id, DEBUG_RENDERING_LOC_NAME);
    m_locations[DEBUG_RENDERING_COLOR_LOC_NAME] = glGetUniformLocation(program_id, DEBUG_RENDERING_COLOR_LOC_NAME);
}

void ShadersDataManager::load_shadow_map_matrix_location(GLuint program_id) {
    m_locations[SHADOW_MAP_DEPTH_VP_MAT_LOC_NAME] = glGetUniformLocation(program_id, SHADOW_MAP_DEPTH_VP_MAT_LOC_NAME);
}

void ShadersDataManager::load_shadow_maps_location(GLuint program_id) {
    m_locations[SHADOW_MAP_ARRAY_LOC_NAME] = glGetUniformLocation(program_id, SHADOW_MAP_ARRAY_LOC_NAME);
}
