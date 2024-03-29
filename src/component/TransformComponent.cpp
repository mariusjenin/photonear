//
// Created by mariusjenin on 24/11/22.
//
#include <utility>
#include "imgui.h"

#include "TransformComponent.h"
#include <ShadersDataManager.h>
#include <Photonear.h>

#define stringify( name ) #name

using namespace component;
using namespace shader_manager;

TransformComponent::TransformComponent(bool displayable) : Component(displayable){
    m_transform = std::make_shared<Transform>();
    m_local_transform = std::make_shared<Transform>();
}


TransformComponent::TransformComponent(std::shared_ptr<Transform> trsf,
                                                  std::shared_ptr<Transform> local_trsf) : Component() {
    m_transform = std::move(trsf);
    m_local_transform = std::move(local_trsf);
}


TransformComponent::TransformComponent(bool displayable, std::shared_ptr<Transform> trsf,
                                       std::shared_ptr<Transform> local_trsf) : Component(displayable) {
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

    Photonear::get_instance()->get_scene()->set_scene_valid(false);
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

mat4 TransformComponent::get_matrix(bool inverse){
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

mat4 TransformComponent::get_matrix_as_end_node(bool inverse){
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
    mat4 model = get_matrix_as_end_node(false);
    mat4 inverse_model = get_matrix_as_end_node(true);
    mat4 normal_model = transpose(inverse_model);
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &model[0][0]);
    glUniformMatrix4fv(shader_data_manager->get_location(ShadersDataManager::NORMAL_MODEL_MAT_LOC_NAME), 1, GL_FALSE,
                       &normal_model[0][0]);

}

void TransformComponent::generate_ui_component_editor() {
    if(!m_displayable) return;
    //Init
    vec3 translation = m_transform->get_translation();
    vec3 rotation = m_transform->get_rotation();
    vec3 scale = m_transform->get_scale();
    OrderRotation order = m_transform->get_order_rotation();
    vec3 local_translation = m_local_transform->get_translation();
    vec3 local_rotation = m_local_transform->get_rotation();
    vec3 local_scale = m_local_transform->get_scale();
    OrderRotation local_order = m_local_transform->get_order_rotation();

    OrderRotation orders[] = { OrderXYZ, OrderXZY, OrderYXZ, OrderYZX, OrderZXY, OrderZYX};


    //UI
    ImGui::PushID(0);
    ImGui::Text("Transform");
    ImGui::DragFloat3("Position",&translation[0],0.1f);
    ImGui::DragFloat3("Rotation",&rotation[0],0.1f);
    ImGui::DragFloat3("Scale",&scale[0],0.01f);
    if (ImGui::BeginCombo("Order", order_to_string(order).c_str())) // The second parameter is the label previewed before opening the combo.
    {
        for (auto & item : orders)
        {
            if (ImGui::Selectable(order_to_string(item).c_str(), (order == item)))
                order = item;
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();
    ImGui::Separator();
    ImGui::PushID(1);
    ImGui::Text("Local Transform");
    ImGui::DragFloat3("Position",&local_translation[0],0.1f);
    ImGui::DragFloat3("Rotation",&local_rotation[0],0.1f);
    ImGui::DragFloat3("Scale",&local_scale[0],0.01f);
    if (ImGui::BeginCombo("Order", order_to_string(local_order).c_str())) // The second parameter is the label previewed before opening the combo.
    {
        for (auto & item : orders)
        {
            if (ImGui::Selectable(order_to_string(item).c_str(), (local_order == item)))
                local_order = item;
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();

    //Attribute Value
    m_transform->set_translation(translation);
    m_transform->set_rotation(rotation);
    m_transform->set_scale(scale);
    m_transform->set_order_rotation(order);
    m_local_transform->set_translation(local_translation);
    m_local_transform->set_rotation(local_rotation);
    m_local_transform->set_scale(local_scale);
    m_local_transform->set_order_rotation(local_order);

    if(!m_transform->is_up_to_date() || !m_local_transform->is_up_to_date()){
        Photonear::get_instance()->get_scene()->set_scene_valid(false);
    }

    if(!m_transform->is_up_to_date()) m_transform->compute();
    if(!m_local_transform->is_up_to_date()) m_local_transform->compute();


}

std::string TransformComponent::order_to_string(OrderRotation order) {
    switch (order) {
        case OrderYXZ:
            return "Y -> X -> Z";
        case OrderXYZ:
            return "X -> Y -> Z";
        case OrderXZY:
            return "X -> Z -> Y";
        case OrderYZX:
            return "Y -> Z -> X";
        case OrderZXY:
            return "Z -> X -> Y";
        case OrderZYX:
            return "Z -> Y -> X";
        default:
            return "";
    }
}