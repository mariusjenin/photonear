//
// Created by mariusjenin on 24/11/22.
//
#include <utility>

#include "TransformComponent.h"
#include <ShadersDataManager.h>


using namespace component;
using namespace shader_manager;

TransformComponent::TransformComponent() {
    m_transform = std::make_shared<Transform>();
    m_local_transform = std::make_shared<Transform>();
}


TransformComponent::TransformComponent(std::shared_ptr<Transform> trsf,
                                                  std::shared_ptr<Transform> local_trsf) {
    m_transform = std::move(trsf);
    m_local_transform = std::move(local_trsf);
}


ComponentType TransformComponent::get_type() {
    return typeid(this);
}


void TransformComponent::compute_trsf_scene_graph() {
    m_local_transform->compute(common::Both);
    m_transform->compute(common::Both);

    auto node = Component::get_node(this);
    compute_trsf_scene_graph_node((AbstractNode*) &*node);
}

void TransformComponent::compute_trsf_scene_graph_node(AbstractNode* node) {
    auto children_trsf_comps = Component::get_children_components<TransformComponent>(&*node);
    if(children_trsf_comps.empty()){
        auto children_node = node->get_children();
        for(const auto& child_node : children_node){
            compute_trsf_scene_graph_node((AbstractNode*) &*child_node);
        }
    } else {
        for (const auto& child_trsf_comp: children_trsf_comps) {
            child_trsf_comp->compute_trsf_scene_graph();
        }
    }
}

glm::mat4 TransformComponent::get_matrix(bool inverse){
    auto node = Component::get_node(this);
    auto trsf_mat = m_transform->get_matrix(inverse);
    auto parent_trsf = Component::get_nearest_component_upper<TransformComponent>(&*node->get_parent());
    if(parent_trsf == nullptr){
        return trsf_mat;
    }
    auto parent_mat = parent_trsf->get_matrix(inverse);
    if(inverse) {
        return trsf_mat * parent_mat;
    }else{
        return parent_mat * trsf_mat;
    }
}

glm::mat4 TransformComponent::get_matrix_as_end_node(bool inverse){
    auto node = Component::get_node(this);
    auto trsf_mat = m_transform->get_matrix(inverse);
    auto local_trsf_mat = m_local_transform->get_matrix(inverse);
    auto parent_trsf = Component::get_nearest_component_upper<TransformComponent>(&*node->get_parent());
    if(parent_trsf == nullptr){
        if(inverse){
            return local_trsf_mat * trsf_mat;
        } else {
            return trsf_mat * local_trsf_mat;
        }
    }
    auto parent_mat = parent_trsf->get_matrix(inverse);
    if(inverse){
        return local_trsf_mat * trsf_mat * parent_mat;
    } else {
        return parent_mat * trsf_mat * local_trsf_mat;
    }
}

std::shared_ptr<Transform> TransformComponent::get_transform() const {
    return m_transform;
}

std::shared_ptr<Transform> TransformComponent::get_local_transform() const {
    return m_local_transform;
}

void TransformComponent::load_in_shaders(const std::shared_ptr<Shaders>& shaders) {
    ShadersDataManager *shader_data_manager = shaders->get_shader_data_manager();

    //Model matrix and Normal model (if non scalar transform)
    glm::mat4 model = get_matrix_as_end_node(false);
    glm::mat4 inverse_model = get_matrix_as_end_node(true);
    glm::mat4 normal_model = glm::transpose(inverse_model);
//    glm::mat4 normal_model = glm::transpose(glm::inverse(model));
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &model[0][0]);
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::NORMAL_MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &normal_model[0][0]);

}