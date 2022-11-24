//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_ROOTNODE_H
#define PHOTONEAR_ROOTNODE_H

#include "AbstractNode.h"
#include "TransformComponent.h"

using namespace component;

namespace scene {
    namespace node {
        /// Root of the %scene graph
        class RootNode : public AbstractNode {
        private:
            std::shared_ptr<TransformComponent> m_trsf_comp;
        public:
            std::shared_ptr<AbstractNode> get_parent() const override;

            void set_trsf_comp(std::shared_ptr<TransformComponent> trsf_comp);

            std::shared_ptr<TransformComponent> get_trsf_comp() const;
        };
    }
}

#endif //PHOTONEAR_ROOTNODE_H
