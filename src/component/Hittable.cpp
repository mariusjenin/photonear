//
// Created by mariusjenin on 19/11/22.
//

#include "Hittable.h"

using namespace component;
using namespace shader_manager;

Hittable::Hittable() {
    m_up_to_date = false;
    m_vertex_positions = {};
    m_triangle_indices = {};
    m_vertex_tex_coords = {};
    m_vertex_normals = {};
}

void Hittable::load_mesh_in_vao() {
    //Generating VAO for this "Mesh" and binding it
    ShadersBufferManager::generate_vao(&m_vao_id);
    ShadersBufferManager::bind_vao(m_vao_id);
    //Generating all the VBO
    ShadersBufferManager::generate_bo(&m_vbo_position_id);
    ShadersBufferManager::generate_bo(&m_vbo_tex_coords_id);
    ShadersBufferManager::generate_bo(&m_vbo_normals_id);
    ShadersBufferManager::generate_bo(&m_ebo_triangle_indices_id);
    //Fill the VBO data
    ShadersBufferManager::fill_bo<glm::vec3>(GL_ARRAY_BUFFER, m_vbo_position_id, m_vertex_positions);
    ShadersBufferManager::fill_bo<glm::vec2>(GL_ARRAY_BUFFER, m_vbo_tex_coords_id, m_vertex_tex_coords);
    ShadersBufferManager::fill_bo<glm::vec3>(GL_ARRAY_BUFFER, m_vbo_normals_id, m_vertex_normals);
    ShadersBufferManager::fill_bo<unsigned short int>(GL_ELEMENT_ARRAY_BUFFER, m_ebo_triangle_indices_id, m_triangle_indices);
    //Attrib Pointers
    ShadersBufferManager::enable_attrib_vbo(ShadersBufferManager::ID_VERTEX_BUFFER, m_vbo_position_id, 3, GL_FALSE);
    ShadersBufferManager::enable_attrib_vbo(ShadersBufferManager::ID_UV_BUFFER, m_vbo_tex_coords_id, 2, GL_FALSE);
    ShadersBufferManager::enable_attrib_vbo(ShadersBufferManager::ID_NORMAL_BUFFER, m_vbo_normals_id, 3, GL_TRUE);
    m_loaded_vao = true;
}

Hittable::~Hittable() {
    if (m_loaded_vao) {
        ShadersBufferManager::disable_attrib_vbo(ShadersBufferManager::ID_VERTEX_BUFFER);
        ShadersBufferManager::disable_attrib_vbo(ShadersBufferManager::ID_UV_BUFFER);
        ShadersBufferManager::disable_attrib_vbo(ShadersBufferManager::ID_NORMAL_BUFFER);
        ShadersBufferManager::delete_bo(m_vbo_position_id);
        ShadersBufferManager::delete_bo(m_vbo_tex_coords_id);
        ShadersBufferManager::delete_bo(m_vbo_normals_id);
        ShadersBufferManager::delete_bo(m_ebo_triangle_indices_id);
        ShadersBufferManager::delete_vao(m_vao_id);
    }
}

GLuint Hittable::get_vao_id() const {
    return m_vao_id;
}

GLuint Hittable::get_ebo_triangle_indices_id() const {
    return m_ebo_triangle_indices_id;
}

std::vector<unsigned short int> Hittable::get_triangle_indices() {
    return m_triangle_indices;
}

ComponentType Hittable::get_type() {
    return typeid(this);
}
