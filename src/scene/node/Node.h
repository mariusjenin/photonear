//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_NODE_H
#define PHOTONEAR_NODE_H

#include <vector>
#include "AbstractNode.h"
#include "Shaders.h"

using namespace shader_manager;

namespace scene {
    namespace node {
        /// Base Node of the scene graph (Abstract)
        class Node : public AbstractNode {
        private:
            std::shared_ptr<AbstractNode> m_parent;
        public:
            /**
             * Constructor of the Node
             * @param parent
             */
            explicit Node(std::shared_ptr<AbstractNode> parent,std::string name);

            /**
             * Setter of the parent AbstractNode
             * @param parent
             */
            void set_parent(std::shared_ptr<AbstractNode> parent);

            std::shared_ptr<AbstractNode> get_parent() const override;

            bool is_active_recursive() override;
        };
    }
}

#endif //PHOTONEAR_NODE_H
