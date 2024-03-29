//
// Created by mariusjenin on 21/11/22.
//

#include "imgui.h"
#include "printer.h"
#include "Camera.h"
#include "TransformComponent.h"
#include "Photonear.h"

using namespace component;
using namespace shader_manager;

void Camera::load_in_shaders(const std::shared_ptr<Shaders> &shaders, int width, int height) {
    auto node_camera = Component::get_node(this);
    auto trsf_comp_camera = Component::get_component<TransformComponent>(&*node_camera);
    Transform trsf = Transform(trsf_comp_camera->get_matrix_as_end_node());

    //PROJECTION
    mat4 projection_mat = perspective(radians(m_fovy), (float) width / (float) height, m_z_near, m_z_far);
    glUniformMatrix4fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);
    // VIEW
    point camera_init_position = CAMERA_POSITION;
    versor camera_init_forward = CAMERA_FORWARD;
    versor camera_init_up = CAMERA_UP;
    point eye_camera = trsf.apply_to_point(camera_init_position);
    versor dir_camera = trsf.apply_to_versor(camera_init_forward);
    versor up_camera = trsf.apply_to_versor(camera_init_up);

    mat4 view_mat = lookAt(
            eye_camera,
            eye_camera + dir_camera,
            up_camera
    );
    glUniformMatrix4fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::VIEW_MAT_LOC_NAME), 1,
                       GL_FALSE, &view_mat[0][0]);

    //POS
    point position_tmp = point(0, 0, 0);
    position_tmp = trsf.apply_to_point(position_tmp);
    glUniform3fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::VIEW_POS_LOC_NAME), 1,
                 &position_tmp[0]);

}

Camera::Camera(bool capturing, float fovy, float z_near, float z_far) : Component() {
    m_fovy = fovy;
    m_z_near = z_near;
    m_z_far = z_far;
    set_capturing(capturing);
}


ComponentType Camera::get_type() {
    return typeid(&*this);
}

void Camera::generate_ui_component_editor() {
    float fovy = m_fovy;
    float z_near = m_z_near;
    float z_far = m_z_far;
    if(!m_capturing){
        if (ImGui::Button("Capture With this camera")) {
            set_capturing(true);
        }
    }
    ImGui::DragFloat("FOV Y", &fovy, 0.1f, 0, 180);
    ImGui::DragFloat("Z Near", &z_near, 0.1f, 0, FLT_MAX);
    ImGui::DragFloat("Z Far", &z_far, 1.f, 0, FLT_MAX);

    if (m_fovy != fovy || m_z_near != z_near || m_z_far != z_far) {
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    }
    if (m_fovy != fovy)m_fovy = fovy;
    if (m_z_near != z_near)m_z_near = z_near;
    if (m_z_far != z_far)m_z_far = z_far;
}

int Camera::is_capturing() const {
    return m_capturing;
}

void Camera::set_capturing(bool capturing) {
    if (m_capturing != capturing)
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    if (capturing) {
        auto cameras = Component::get_components<Camera>();
        for (const auto &camera: cameras) {
            if (&*camera != this) {
                camera->set_capturing(false);
            }
        }
    }
    m_capturing = capturing;
}

float Camera::get_fovy() const {
    return m_fovy;
}

float Camera::get_z_near() const {
    return m_z_near;
}

float Camera::get_z_far() const {
    return m_z_far;
}
