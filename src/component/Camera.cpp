//
// Created by mariusjenin on 21/11/22.
//

#include "Camera.h"

using namespace component;
using namespace shader_manager;

void Camera::update_view_mat(Transform trsf, std::shared_ptr<VertFragShaders> shaders) {
    glm::vec3 camera_init_position = CAMERA_POSITION;
    glm::vec3 camera_init_forward = CAMERA_FORWARD;
    glm::vec3 camera_init_up = CAMERA_UP;
    glm::vec3 eye_camera = trsf.apply_to_point(camera_init_position);
    glm::vec3 dir_camera = trsf.apply_to_versor(camera_init_forward);
    glm::vec3 up_camera = trsf.apply_to_versor(camera_init_up);

    glm::mat4 view_mat = lookAt(
            eye_camera,
            eye_camera + dir_camera,
            up_camera
    );
    glUniformMatrix4fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::VIEW_MAT_LOC_NAME), 1,
                       GL_FALSE, &view_mat[0][0]);
}

void Camera::update_view_pos(Transform trsf, std::shared_ptr<VertFragShaders> shaders) {
    glm::vec3 position_tmp = glm::vec3(0, 0, 0);
    position_tmp = trsf.apply_to_point(position_tmp);
    glUniform3fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::VIEW_POS_LOC_NAME), 1,
                 &position_tmp[0]);
}

Camera::Camera(int priority) {
    m_priority = priority;
}

int Camera::get_priority() const {
    return m_priority;
}

ComponentType Camera::get_type() {
    return typeid(this);
}
