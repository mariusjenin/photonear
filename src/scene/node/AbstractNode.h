//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_ABSTRACTNODE_H
#define PHOTONEAR_ABSTRACTNODE_H


#include <map>
#include <utility>
#include <memory>
#include "Transform.h"

using namespace common;

namespace scene {
    namespace node {
        class Node;

        /// Base Element of the scene graph (Abstract)
        class AbstractNode {
        protected:
            /// Transformation applied to the AbstractNode and all the children
            std::vector<std::shared_ptr<Node>> m_children{};
            bool m_children_dirty;
        public :

            /**
             * Add a child to the AbstractNode
             * @param node
             */
            void add_child(std::shared_ptr<Node> node);

            /**
             * Constructor of the AbstractNode
             */
            explicit AbstractNode();


            /**
             * Getter of all the children Node
             * @return children
             */
            std::vector<std::shared_ptr<Node>> get_children() const;

            /**
             * Getter of the parent AbstractNode
             * @return parent
             */
            virtual std::shared_ptr<AbstractNode> get_parent() const = 0;

            /**
             * Remove a child from the list of Node at a given position
             * @param i
             */
            void remove_child_at(int i);

            /**
             * Clear the list of Node
             */
            void clear_children();

            /**
             * Getter of whether or not the AbstractNode has children
             * @return has children
             */
            bool has_children() const;

            /// Destructor of the AbstractNode
            virtual ~AbstractNode();
        };
    }
}

#endif //PHOTONEAR_ABSTRACTNODE_H