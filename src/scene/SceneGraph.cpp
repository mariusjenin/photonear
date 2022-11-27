//
// Created by mariusjenin on 21/11/22.
//

#include "SceneGraph.h"

#include <utility>
#include "Component.h"
#include "TransformComponent.h"

using namespace scene;
using namespace component;

SceneGraph::SceneGraph(std::shared_ptr<RootNode> rn) {
    m_root_node = std::move(rn);
    auto trsf_comp_root = std::make_shared<TransformComponent>();
    m_root_node->set_trsf_comp(trsf_comp_root);
    Component::add_component_to_node(trsf_comp_root,m_root_node);
}

std::shared_ptr<RootNode> SceneGraph::get_root_node() {
    return m_root_node;
}

void SceneGraph::compute_scene_graph() {
    auto trsf_comp = m_root_node->get_trsf_comp();
    trsf_comp->compute_trsf_scene_graph();
}
