//
// Created by mariusjenin on 21/11/22.
//

#include "ShadowMapShaders.h"

using namespace shader_manager;

ShadowMapShaders::ShadowMapShaders(const char *vertex_file_path, const char *fragment_file_path) : Shaders(
        vertex_file_path, fragment_file_path, false) {}

void ShadowMapShaders::load_location() const {
    m_shader_data_manager->load_shadow_map_matrix_location(m_program_id);
    m_shader_data_manager->load_matrices_locations(m_program_id);
}
