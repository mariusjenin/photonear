//
// Created by mariusjenin on 19/11/22.
//

#include <utility>
#include "glm/glm.hpp"
#include "imgui.h"

#include "AbstractNode.h"
#include "Node.h"
#include "Photonear.h"


using namespace scene::node;

AbstractNode::AbstractNode(std::string name) {
    m_children = {};
    m_children_dirty = true;
    m_name = std::move(name);
}

void AbstractNode::add_child(const std::shared_ptr<Node>& node) {
    m_children.push_back(node);
    m_children_dirty = true;
}

AbstractNode::~AbstractNode() {
    m_children.clear();
}

std::vector<std::shared_ptr<Node>> AbstractNode::get_children() const{
    return m_children;
}


void AbstractNode::remove_child_at(int i) {
    m_children.erase(m_children.begin()+i);
    m_children_dirty = true;
}

void AbstractNode::clear_children() {
    m_children.clear();
    m_children_dirty = true;
}

bool AbstractNode::has_children() const {
    return !m_children.empty();
}

void AbstractNode::generate_scene_graph_ui() {

    auto flags_leaf = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth;

    auto photonear = Photonear::get_instance();
    auto node_selected = photonear->get_node_selected();

    int size_children = (int) m_children.size();
    if(size_children > 0){
        auto tree_node_open = ImGui::TreeNodeEx(this,ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow ,"");
        ImGui::SameLine();
        ImGui::PushID(this);
        if (ImGui::Selectable(m_name.c_str(),node_selected == this,flags_leaf)){
            photonear->set_node_selected(this);
        }
        ImGui::PopID();
        if (tree_node_open){
            for(const auto& child : m_children){
                child->generate_scene_graph_ui();
            }
            ImGui::TreePop();
        }
    } else {
        ImGui::PushID(this);
        if (ImGui::Selectable(m_name.c_str(),node_selected == this,flags_leaf)){
            photonear->set_node_selected(this);
        }
        ImGui::PopID();
    }
}

std::string AbstractNode::get_name() const {
    return m_name;
}
