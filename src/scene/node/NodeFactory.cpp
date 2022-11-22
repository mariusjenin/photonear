//
// Created by mariusjenin on 19/11/22.
//

#include "NodeFactory.h"
#include "AbstractNode.h"
#include "RootNode.h"
#include "Node.h"

using namespace scene::node;

std::shared_ptr<RootNode> NodeFactory::create_root_node() {
    return std::make_shared<RootNode>();
}

std::shared_ptr<Node> NodeFactory::create_node(std::shared_ptr<AbstractNode> parent) {
    auto node = std::make_shared<Node>(parent);
    parent->add_child(node);
    return node;
}
