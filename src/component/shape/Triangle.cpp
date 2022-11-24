//
// Created by mariusjenin on 21/11/22.
//

#include "Triangle.h"

using namespace component::shape;

Triangle::Triangle(bool both_face_visible) : Shape(
        both_face_visible) {
    assign_mesh_triangle();
    load_mesh_in_vao();
}

void Triangle::assign_mesh_triangle() {
    m_vertex_positions.clear();
    m_vertex_normals.clear();
    m_vertex_tex_coords.clear();
    m_triangle_indices.clear();


    m_vertex_positions.emplace_back(-1,-1,0);
    m_vertex_positions.emplace_back(-1,1,0);
    m_vertex_positions.emplace_back(1,-1,0);
//    m_vertex_positions.emplace_back(1,1,0);

    m_vertex_normals.emplace_back(0,0,1);
    m_vertex_normals.emplace_back(0,0,1);
    m_vertex_normals.emplace_back(0,0,1);
//    m_vertex_normals.emplace_back(0,0,1);

    m_vertex_tex_coords.emplace_back(0,0);
    m_vertex_tex_coords.emplace_back(0,1);
    m_vertex_tex_coords.emplace_back(1,0);
//    m_vertex_tex_coords.emplace_back(1,1);

    m_triangle_indices.emplace_back(1);
    m_triangle_indices.emplace_back(0);
    m_triangle_indices.emplace_back(2);
//    m_triangle_indices.emplace_back(1);
//    m_triangle_indices.emplace_back(2);
//    m_triangle_indices.emplace_back(3);
}
