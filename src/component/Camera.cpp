//
// Created by mariusjenin on 21/11/22.
//

#include "Camera.h"
#include "TransformComponent.h"
#include "printer.h"

using namespace component;
using namespace shader_manager;

void Camera::load_in_shaders(GLFWwindow *window, const std::shared_ptr<VertFragShaders>& shaders) {
    auto node_camera = Component::get_node(this);
    auto trsf_comp_camera = Component::get_component<TransformComponent>(&*node_camera);
    Transform trsf = Transform(trsf_comp_camera->get_matrix_as_end_node());

    //PROJECTION
    int width,height;
    glfwGetWindowSize(window, &width, &height);

//    std::cout << width<< " " << height <<" "<<m_fovy <<" "<<m_z_near <<" "<<m_z_far << std::endl;

    mat4 projection_mat = perspective(radians(m_fovy), (float)width/(float)height, m_z_near, m_z_far);
    glUniformMatrix4fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);
    // VIEW
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

    //POS
    glm::vec3 position_tmp = glm::vec3(0, 0, 0);
    position_tmp = trsf.apply_to_point(position_tmp);
    glUniform3fv(shaders->get_shader_data_manager()->get_location(ShadersDataManager::VIEW_POS_LOC_NAME), 1,
                 &position_tmp[0]);

}

Camera::Camera(int priority, float fovy, float z_near, float z_far) {
    m_priority = priority;
    m_fovy = fovy;
    m_z_near = z_near;
    m_z_far = z_far;
}

int Camera::get_priority() const {
    return m_priority;
}

ComponentType Camera::get_type() {
    return typeid(&*this);
}

