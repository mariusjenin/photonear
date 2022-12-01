//
// Created by mariusjenin on 21/11/22.
//

#include "imgui.h"

#include "Quad.h"
#include "Photonear.h"

using namespace component::shape;

Quad::Quad(float length_x, float length_z, bool both_face_visible) : Shape(both_face_visible){
    m_normal = {0,1,0};
    m_length_x = length_x;
    m_length_z = length_z;
    assign_mesh_plane();
    load_mesh_in_vao();
}

void Quad::assign_mesh_plane() {
    m_vertex_positions.clear();
    m_vertex_normals.clear();
    m_vertex_tex_coords.clear();
    m_triangle_indices.clear();

    glm::vec3 points[4];

    auto half_length_x = m_length_x/2.f;
    auto half_length_z = m_length_z/2.f;
    points[0] = glm::vec3(-half_length_x,0,-half_length_z);
    points[1] = glm::vec3(-half_length_x,0,half_length_z);
    points[2] = glm::vec3(half_length_x,0,half_length_z);
    points[3] = glm::vec3(half_length_x,0,-half_length_z);

    for(auto point:points){
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

void Quad::generate_ui_component_editor() {
    Shape::generate_ui_component_editor();
    float length_x = m_length_x;
    float length_z = m_length_z;
    ImGui::DragFloat("Length X",&length_x,0.01f,0,FLT_MAX);
    ImGui::DragFloat("Length Z",&length_z,0.01f,0,FLT_MAX);
    auto length_x_changed = length_x != m_length_x;
    auto length_z_changed = length_z != m_length_z;
    if(length_x_changed) m_length_x = length_x;
    if(length_z_changed) m_length_z = length_z;
    if(length_x_changed || length_z_changed){
        assign_mesh_plane();
        load_mesh_in_vao();
        Photonear::get_instance()->get_scene()->set_scene_valid();
    }
}



std::vector<glm::vec3> Quad::to_few_vertices() {
    auto bb = BoundingBox();
    auto half_length_x = m_length_x/2.f;
    auto half_length_z = m_length_z/2.f;
    glm::vec3 min = {-half_length_x,0,-half_length_z};
    glm::vec3 max = {half_length_x,0,half_length_z};
    return {min,
            {min[0], min[1], max[2]},
            {min[0], max[1], min[2]},
            {min[0], max[1], max[2]},
            {max[0], min[1], min[2]},
            {max[0], min[1], max[2]},
            {max[0], max[1], min[2]},
            max
    };
}
