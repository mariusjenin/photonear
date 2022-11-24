//
// Created by mariusjenin on 19/11/22.
//

#include "AbstractNode.h"
#include "Node.h"
#include "glm/glm.hpp"
#include <utility>


using namespace scene::node;

AbstractNode::AbstractNode() {
    m_children = {};
    m_children_dirty = true;
}

void AbstractNode::add_child(std::shared_ptr<Node> node) {
    m_children.push_back(node);
    m_children_dirty = true;
}
//
//void AbstractNode::draw(Shaders *shaders, glm::vec3 pos_camera, bool allow_debug) {
//    for (auto &child: m_children) {
//        child->draw(shaders,pos_camera, allow_debug);
//    }
//}

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