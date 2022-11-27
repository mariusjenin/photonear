//
// Created by mariusjenin on 21/11/22.
//

#include "Quad.h"

using namespace component::shape;

Quad::Quad(glm::vec3 top_left_pt, glm::vec3 bot_left_pt, glm::vec3 bot_right_pt, bool both_face_visible) : Shape(
        both_face_visible) {
    vec3 right = bot_right_pt - bot_left_pt;
    vec3 top = top_left_pt - bot_left_pt;
    m_normal = glm::cross(right,top);
    m_points[0] = top_left_pt;
    m_points[1] = bot_left_pt;
    m_points[2] = bot_right_pt;
    m_points[3] = top_left_pt + right;

    assign_mesh_plane();
    load_mesh_in_vao();
}

void Quad::assign_mesh_plane() {
    m_vertex_positions.clear();
    m_vertex_normals.clear();
    m_vertex_tex_coords.clear();
    m_triangle_indices.clear();

    for(auto point:m_points){
        m_vertex_positions.emplace_back(point[0],point[1],point[2]);
        m_vertex_normals.emplace_back(m_normal[0],m_normal[1],m_normal[2]);
    }

    m_vertex_tex_coords.emplace_back(0,1);
    m_vertex_tex_coords.emplace_back(0,0);
    m_vertex_tex_coords.emplace_back(1,0);
    m_vertex_tex_coords.emplace_back(1,1);

    m_triangle_indices.emplace_back(0);
    m_triangle_indices.emplace_back(1);
    m_triangle_indices.emplace_back(2);
    m_triangle_indices.emplace_back(2);
    m_triangle_indices.emplace_back(3);
    m_triangle_indices.emplace_back(0);
}
