//
// Created by mariusjenin on 19/11/22.
//

#include "RootNode.h"

#include <utility>
#include "Component.h"


using namespace scene::node;
using namespace component;


std::shared_ptr<AbstractNode> RootNode::get_parent() const {
    return nullptr;
}

void RootNode::set_trsf_comp(std::shared_ptr<TransformComponent> trsf_comp) {
    m_trsf_comp = std::move(trsf_comp);
}

std::shared_ptr<TransformComponent> RootNode::get_trsf_comp() const {
    return m_trsf_comp;
}
