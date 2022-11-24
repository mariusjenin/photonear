//
// Created by mariusjenin on 19/11/22.
//

#include "Node.h"

#include <utility>


using namespace scene::node;

Node::Node(std::shared_ptr<AbstractNode> parent)
        : AbstractNode() {
    m_parent = std::move(parent);
}

void Node::set_parent(std::shared_ptr<AbstractNode> parent){
    m_parent = std::move(parent);
}


std::shared_ptr<AbstractNode> Node::get_parent() const {
    return m_parent;
}


