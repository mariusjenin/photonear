//
// Created by mariusjenin on 21/11/22.
//

#include "imgui.h"

#include "Sphere.h"

using namespace component::shape;


Sphere::Sphere(float radius, int slices, int stacks, bool both_face_visible) : Shape(
        both_face_visible) {
    m_radius = radius;
    m_slices = slices;
    m_stacks = stacks;
    assign_mesh_sphere();
    load_mesh_in_vao();
}

void Sphere::assign_mesh_sphere() {
    m_vertex_positions.clear();
    m_vertex_normals.clear();
    m_vertex_tex_coords.clear();
    m_triangle_indices.clear();

    float du = 2.f * (float) M_PI / (float) m_slices;
    float dv = (float) M_PI / (float) m_stacks;
    int i, j;
    float u, v;
    vec3 coords;
    for (i = 0; i <= m_stacks; i++) {
        v = (float) -M_PI / 2 + (float) i * dv;
        for (j = 0; j <= m_slices; j++) {
            u = (float) j * du;
            coords = spheric_to_euclidian_coords(u, v);
            m_vertex_positions.emplace_back(m_radius * coords.x, m_radius * coords.y, m_radius * coords.z);
            m_vertex_normals.emplace_back(m_radius * coords.x, m_radius * coords.y, m_radius * coords.z);
            m_vertex_tex_coords.emplace_back((float) j / (float) m_slices, (float) i / (float) m_stacks);
        }
    }
    for (j = 0; j < m_stacks; j++) {
        int row1 = j * (m_slices + 1);
        int row2 = (j + 1) * (m_slices + 1);
        for (i = 0; i < m_slices; i++) {
            m_triangle_indices.push_back(row1 + i);
            m_triangle_indices.push_back(row2 + i + 1);
            m_triangle_indices.push_back(row2 + i);
            m_triangle_indices.push_back(row1 + i);
            m_triangle_indices.push_back(row1 + i + 1);
            m_triangle_indices.push_back(row2 + i + 1);
        }
    }
}

void Sphere::generate_component_editor_ui() {
    float radius = m_radius;
    int slices = m_slices;
    int stacks = m_stacks;
    ImGui::DragFloat("Radius",&radius,0.01f,0,FLT_MAX);
    ImGui::DragInt("Slices",&slices,0.1f,0,INT_MAX);
    ImGui::DragInt("Stacks",&stacks,0.1f,0,INT_MAX);
    ImGui::Checkbox("Both Face Visible",&m_both_face_visible);
    auto radius_changed = radius != m_radius;
    auto slices_changed = slices != m_slices;
    auto stacks_changed = stacks != m_stacks;
    if(radius_changed) m_radius = radius;
    if(slices_changed) m_slices = slices;
    if(stacks_changed) m_stacks = stacks;
    if(radius_changed || slices_changed || stacks_changed){
        assign_mesh_sphere();
        load_mesh_in_vao();
    }
}
