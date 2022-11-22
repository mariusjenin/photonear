//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_ROOTNODE_H
#define PHOTONEAR_ROOTNODE_H

#include "AbstractNode.h"

namespace scene {
    namespace node {
        /// Root of the %scene graph
        class RootNode : public AbstractNode {
        public:
            glm::mat4 get_matrix_recursive(bool inverse) override;
            glm::mat4 get_matrix_recursive_local(bool inverse) override;
        };
    }
}

#endif //PHOTONEAR_ROOTNODE_H
