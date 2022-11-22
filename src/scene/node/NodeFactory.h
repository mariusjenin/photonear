//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_NODEFACTORY_H
#define PHOTONEAR_NODEFACTORY_H

#include "memory"
//#include "AbstractNode.h"
//#include "RootNode.h"

namespace scene{
    namespace node{
        class RootNode;
        class Node;
        class AbstractNode;
        class NodeFactory {
        public:
            static std::shared_ptr<RootNode> create_root_node();
            static std::shared_ptr<Node> create_node(std::shared_ptr<AbstractNode> parent);
        };

    }
}

#endif //PHOTONEAR_NODEFACTORY_H
