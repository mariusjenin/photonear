//
// Created by mariusjenin on 21/11/22.
//

#include "Sphere.h"

using namespace component;

Sphere::Sphere(glm::vec3 center, float radius, int slices, int stacks) {
    m_center = center;
    m_radius = radius;
    assign_mesh_sphere(radius,slices,stacks);
}

void Sphere::assign_mesh_sphere(float radius, int slices, int stacks) {
    m_vertex_positions.clear();
    m_vertex_normals.clear();
    m_vertex_tex_coords.clear();
    m_triangle_indices.clear();

    float du = 2.f * (float) M_PI / (float) slices;
    float dv = (float) M_PI / (float) stacks;
    int i, j;
    float u, v;
    vec3 coords;
    for (i = 0; i <= stacks; i++) {
        v = (float) -M_PI / 2 + (float) i * dv;
        for (j = 0; j <= slices; j++) {
            u = (float) j * du;
            coords = spheric_to_euclidian_coords(u, v);
//                x = cos(u) * cos(v);
//                y = sin(u) * cos(v);
//                z = sin(v);
            m_vertex_positions.emplace_back(radius * coords.x, radius * coords.y, radius * coords.z);
            m_vertex_normals.emplace_back(radius * coords.x, radius * coords.y, radius * coords.z);
            m_vertex_tex_coords.emplace_back((float) j / (float) slices, (float) i / (float) stacks);
        }
    }
    for (j = 0; j < stacks; j++) {
        int row1 = j * (slices + 1);
        int row2 = (j + 1) * (slices + 1);
        for (i = 0; i < slices; i++) {
            m_triangle_indices.push_back(row1 + i);
            m_triangle_indices.push_back(row2 + i + 1);
            m_triangle_indices.push_back(row2 + i);
            m_triangle_indices.push_back(row1 + i);
            m_triangle_indices.push_back(row1 + i + 1);
            m_triangle_indices.push_back(row2 + i + 1);
        }
    }
}
