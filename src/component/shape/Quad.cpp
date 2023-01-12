//
// Created by mariusjenin on 21/11/22.
//

#include "imgui.h"

#include "Quad.h"
#include "Photonear.h"

using namespace component::shape;

Quad::Quad(float length_x, float length_z, bool both_face_visible,float accepted_normal_angle) : Shape(both_face_visible){
    m_normal = {0,1,0};
    m_length_x = length_x;
    m_length_z = length_z;
    assign_mesh_plane();
    load_mesh_in_vao();
    m_accepted_normal_angle = accepted_normal_angle;
}

void Quad::assign_mesh_plane() {
    m_vertex_positions.clear();
    m_vertex_normals.clear();
    m_vertex_tex_coords.clear();
    m_triangle_indices.clear();


    auto half_length_x = m_length_x/2.f;
    auto half_length_z = m_length_z/2.f;

    point points[4];
    points[0] = point(-half_length_x,0,-half_length_z);
    points[1] = point(-half_length_x,0,half_length_z);
    points[2] = point(half_length_x,0,half_length_z);
    points[3] = point(half_length_x,0,-half_length_z);

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
    float accepted_normal_angle = m_accepted_normal_angle;
    ImGui::DragFloat("Length X",&length_x,0.01f,0,FLT_MAX);
    ImGui::DragFloat("Length Z",&length_z,0.01f,0,FLT_MAX);
    ImGui::DragFloat("Accepted angle normal",&accepted_normal_angle,0.1f,0,180);
    auto length_x_changed = length_x != m_length_x;
    auto length_z_changed = length_z != m_length_z;
    auto accepted_normal_angle_changed = accepted_normal_angle != m_accepted_normal_angle;
    if(length_x_changed) m_length_x = length_x;
    if(length_z_changed) m_length_z = length_z;
    if(accepted_normal_angle_changed) m_accepted_normal_angle = accepted_normal_angle;
    if(length_x_changed || length_z_changed || accepted_normal_angle_changed){
        assign_mesh_plane();
        load_mesh_in_vao();
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    }
}



std::vector<point> Quad::to_few_vertices() {
    auto bb = BoundingBox();
    auto half_length_x = m_length_x/2.f;
    auto half_length_z = m_length_z/2.f;
    point min = {-half_length_x,0,-half_length_z};
    point max = {half_length_x,0,half_length_z};
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

RayCastHit Quad::hit(Ray ray) {
    RayCastHit ray_hit = RayCastHit();

    auto half_length_x = m_length_x/2.f;
    auto half_length_z = m_length_z/2.f;

    auto node = Component::get_node(this);
    auto matrix = Component::get_component<TransformComponent>(&*node)->get_matrix_as_end_node();

    auto origin = ray.get_origin();
    auto direction = ray.get_direction();
    auto normal_quad = normalize(vec3(matrix * vec4(0,1,0,0)));

    if(dot(direction,normal_quad) > 0 && !m_both_face_visible){
        return ray_hit;
    } else {
        normal_quad *= -1;
    }

    point pt_quad = point(matrix * vec4(-half_length_x,0,-half_length_z,1));

    point points[4];
    points[0] = point(-half_length_x,0,-half_length_z);
    points[1] = point(-half_length_x,0,half_length_z);
    points[2] = point(half_length_x,0,half_length_z);
    points[3] = point(half_length_x,0,-half_length_z);

    auto right_vector = (vec3(matrix * vec4(points[1] - points[0],0)));
    auto up_vector = (vec3(matrix * vec4(points[3] - points[0],0)));

    float right_vector_norm = length(right_vector);
    float up_vector_norm = length(up_vector);

    float t = (dot(pt_quad,normal_quad) - dot(origin,normal_quad)) /  dot(direction,normal_quad);
    if (t < ray.get_t_min() || ray.get_t_max() < t) {
        return ray_hit;
    }
    ray_hit.t = t;

    auto intersection = origin + t * direction;

    auto v = intersection - pt_quad;
    float right_val = dot(v,right_vector);
    float up_val = dot(v,up_vector);

    if(right_val >= 0 && up_val >= 0 &&
       right_val <= right_vector_norm*right_vector_norm &&
       up_val <= up_vector_norm*up_vector_norm){
        ray_hit.hit = true;
        ray_hit.normal = normal_quad;
        ray_hit.hit_point = intersection;
        ray_hit.shape = this;
        ray_hit.inner_shape = false;
        ray_hit.u = (right_val / right_vector_norm) / right_vector_norm;
        ray_hit.v = (up_val / up_vector_norm) / up_vector_norm;
    }
    return ray_hit;
}

bool Quad::normal_test(std::shared_ptr<RayTraceHit> ray_hit, std::shared_ptr<Photon> photon) {
    if(m_accepted_normal_angle == 0) return photon->normal == ray_hit->normal;
    return dot(photon->normal, ray_hit->normal) > cos(m_accepted_normal_angle * M_PI / 180.f);
}
