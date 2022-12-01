//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_NODEFACTORY_H
#define PHOTONEAR_NODEFACTORY_H

#include "memory"

namespace scene{
    namespace node{
        class RootNode;
        class Node;
        class AbstractNode;
        class NodeFactory {
        private:
            static int NodeCount;
        public:
            static std::shared_ptr<RootNode> create_root_node(const std::string& name = "RootNode");
            static std::shared_ptr<Node> create_node(const std::shared_ptr<AbstractNode>& parent, std::string name = "");
        };

    }
}

#endif //PHOTONEAR_NODEFACTORY_H
