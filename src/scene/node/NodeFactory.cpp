//
// Created by mariusjenin on 19/11/22.
//

#include "NodeFactory.h"
#include "AbstractNode.h"
#include "RootNode.h"
#include "Node.h"

using namespace scene::node;

std::shared_ptr<RootNode> NodeFactory::create_root_node() {
    // The TransformComponent is creating in the SceneGraph Constructor
    return std::make_shared<RootNode>();
}

std::shared_ptr<Node> NodeFactory::create_node(const std::shared_ptr<AbstractNode>& parent, bool with_transform_compnent) {
    auto node = std::make_shared<Node>(parent);
    parent->add_child(node);
    if(with_transform_compnent){
        Component::add_component_to_node<TransformComponent>(std::make_shared<TransformComponent>(),node);
    }
    return node;
}
