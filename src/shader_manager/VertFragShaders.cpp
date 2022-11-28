//
// Created by mariusjenin on 21/11/22.
//

#include "VertFragShaders.h"

using namespace shader_manager;

VertFragShaders::VertFragShaders(const char *vertex_file_path, const char *fragment_file_path) : Shaders(
        vertex_file_path, fragment_file_path, true) {
    m_shadow_map_shaders = std::make_shared<ShadowMapShaders>("../src/shader/depth_vertex_shader.glsl","../src/shader/depth_fragment_shader.glsl");
}


void VertFragShaders::load_location() const {
    m_shader_data_manager->load_matrices_locations(m_program_id);
    m_shader_data_manager->load_view_pos_location(m_program_id);
    m_shader_data_manager->load_lights_const(m_program_id);
    m_shader_data_manager->load_material_const(m_program_id);
    m_shader_data_manager->load_material_locations(m_program_id);
    m_shader_data_manager->load_lights_locations(m_program_id);
    m_shader_data_manager->load_shadow_maps_location(m_program_id);
}

std::shared_ptr<ShadowMapShaders> VertFragShaders::get_shadow_map_shaders() const {
    return m_shadow_map_shaders;
}
