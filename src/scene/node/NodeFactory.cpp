//
// Created by mariusjenin on 19/11/22.
//

#include "NodeFactory.h"
#include "AbstractNode.h"
#include "RootNode.h"
#include "Node.h"
#include "BoundingBox.h"

using namespace scene::node;

int NodeFactory::NodeCount = 1;

std::shared_ptr<RootNode> NodeFactory::create_root_node(const std::string& name) {
    return std::make_shared<RootNode>(name);
}

std::shared_ptr<Node> NodeFactory::create_node(const std::shared_ptr<AbstractNode>& parent, std::string name) {
    if(name.empty()){
        name = "Node"+std::to_string(NodeCount);
        NodeCount++;
    }
    auto node = std::make_shared<Node>(parent,name);
    parent->add_child(node);

    Component::add_component_to_node(std::make_shared<TransformComponent>(),node);
    return node;
}
