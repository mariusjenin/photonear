//
// Created by mariusjenin on 19/11/22.
//

#include "Node.h"

#include <utility>


using namespace scene::node;

Node::Node(std::shared_ptr<AbstractNode> parent,std::string name)
        : AbstractNode(std::move(name)) {
    m_parent = std::move(parent);
}

void Node::set_parent(std::shared_ptr<AbstractNode> parent){
    m_parent = std::move(parent);
}


std::shared_ptr<AbstractNode> Node::get_parent() const {
    return m_parent;
}

bool Node::is_active_recursive() {
    if(!m_active) return false;
    else return m_parent->is_active_recursive();
}


