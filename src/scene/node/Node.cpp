//
// Created by mariusjenin on 19/11/22.
//

#include "Node.h"

#include <utility>


using namespace scene::node;

Node::Node(std::shared_ptr<AbstractNode> parent)
        : AbstractNode() {
    m_parent = parent;
    m_local_trsf = new Transform();
}

glm::mat4 Node::get_matrix_recursive(bool inverse) {
    if(inverse) {
        return m_trsf->get_inverse() * m_parent->get_matrix_recursive(inverse);
    }else{
        return m_parent->get_matrix_recursive(inverse) * m_trsf->get_matrix();
    }
}

glm::mat4 Node::get_matrix_recursive_local(bool inverse) {
    if(inverse){
        return m_local_trsf->get_inverse() * m_trsf->get_inverse() * m_parent->get_matrix_recursive(inverse);
    } else {
        return m_parent->get_matrix_recursive(false) * m_trsf->get_matrix() * m_local_trsf->get_matrix();
    }
}

glm::vec3 Node::get_position_in_world(glm::vec3 center) {
    Transform trsf = Transform();
    trsf.set_matrix(get_matrix_recursive_local(false));
    return trsf.apply_to_point(center);
}

void Node::compute_trsf_scene_graph() {

    m_local_trsf->compute();

    AbstractNode::compute_trsf_scene_graph();
}

Transform *Node::get_local_trsf() {
    return m_local_trsf;
}

void Node::load_model_matrices(Shaders* shaders) {
    ShadersDataManager *shader_data_manager = shaders->get_shader_data_manager();

    //Model matrix and Normal model (if non scalar transform)
    glm::mat4 model = get_matrix_recursive_local(false);
    glm::mat4 normal_model = glm::transpose(glm::inverse(model));
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &model[0][0]);
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::NORMAL_MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &normal_model[0][0]);
}

void Node::set_parent(std::shared_ptr<AbstractNode> parent){
    m_parent = parent;
}

Node::~Node() {
    delete m_local_trsf;
}


