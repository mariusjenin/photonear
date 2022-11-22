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
        protected:

            Transform *m_local_trsf;
        public:
            /**
             * Constructor of the Node
             * @param parent
             */
            explicit Node(std::shared_ptr<AbstractNode> parent);

            /**
             * Load the Model matrices of the Node in the Shaders
             * @param shaders
             */
            void load_model_matrices(Shaders *shaders);

            /**
             * Getter of the local transformation matrix of the Node
             * @return trsf
             */
            Transform *get_local_trsf();


            glm::mat4 get_matrix_recursive(bool inverse) override;


            glm::mat4 get_matrix_recursive_local(bool inverse) override;


            void compute_trsf_scene_graph() override;


            /**
             * Compute the position in the world of the Node
             * @param v
             * @return position
             */
            glm::vec3 get_position_in_world(glm::vec3 v = {0, 0, 0});

            /**
             * Setter of the parent AbstractNode
             * @param parent
             */
            void set_parent(std::shared_ptr<AbstractNode> parent);

            ~Node() override;
        };
    }
}

#endif //PHOTONEAR_NODE_H
