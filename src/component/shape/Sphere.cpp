//
// Created by mariusjenin on 21/11/22.
//

#include "imgui.h"

#include "Sphere.h"
#include "Photonear.h"

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
    point coords;
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

void Sphere::generate_ui_component_editor() {
    Shape::generate_ui_component_editor();
    float radius = m_radius;
    int slices = m_slices;
    int stacks = m_stacks;
    ImGui::DragFloat("Radius",&radius,0.01f,0,FLT_MAX);
    ImGui::DragInt("Slices",&slices,0.1f,0,INT_MAX);
    ImGui::DragInt("Stacks",&stacks,0.1f,0,INT_MAX);
    auto radius_changed = radius != m_radius;
    auto slices_changed = slices != m_slices;
    auto stacks_changed = stacks != m_stacks;
    if(radius_changed) m_radius = radius;
    if(slices_changed) m_slices = slices;
    if(stacks_changed) m_stacks = stacks;
    if(radius_changed || slices_changed || stacks_changed){
        assign_mesh_sphere();
        load_mesh_in_vao();
        if(slices_changed || stacks_changed){
            Photonear::get_instance()->get_scene()->set_viewer_valid();
        }
        if(radius_changed){
            Photonear::get_instance()->get_scene()->set_scene_valid();

        }
    }
}



std::vector<point> Sphere::to_few_vertices() {
    vec3 radius_vec = {m_radius,m_radius,m_radius};
    point min = m_center-radius_vec;
    point max = m_center+radius_vec;
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

RayTraceHit Sphere::hit(Ray ray) {
    RayTraceHit ray_hit = RayTraceHit();
    auto node = Component::get_node(this);
    auto matrix = Component::get_component<TransformComponent>(&*node)->get_matrix_as_end_node();
    auto center = vec3(matrix*vec4(m_center,1));
    auto ray_dir = ray.get_direction();
    auto sqr_length_ray_dir = glm::dot(ray_dir,ray_dir);
    auto ray_origin = ray.get_origin();
    auto oc = ray_origin - center;
    auto sqr_length_oc = glm::dot(oc,oc);
    auto a = sqr_length_ray_dir;
    auto half_b = dot(oc, ray_dir);
    auto c = sqr_length_oc - m_radius*m_radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) {
        return ray_hit;
    }

    auto sqrt_d = sqrt(discriminant);
    float t_min = ray.get_t_min();
    float t_max = ray.get_t_max();

    auto root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root)
            return ray_hit;
    }

    ray_hit.hit = true;
    ray_hit.t = root;
    ray_hit.hit_point = ray_origin + ray_hit.t * ray_dir;
    ray_hit.normal = (ray_hit.hit_point - center) / m_radius;

    float pi = M_PI;
    float theta = acos(-ray_hit.normal.y);
    float phi = atan2(-ray_hit.normal.z, ray_hit.normal.x) + pi;
    ray_hit.u = phi / (2.f*pi);
    ray_hit.v = theta / pi;

    if(dot(ray_dir, ray_hit.normal) >= 0){
        if(m_both_face_visible){
            ray_hit.normal *= -1;
        } else {
            ray_hit.hit = false;
            return ray_hit;
        }
    }
    ray_hit.shape = this;
    return ray_hit;
}
