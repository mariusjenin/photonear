//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_ABSTRACTNODE_H
#define PHOTONEAR_ABSTRACTNODE_H


#include <map>
#include <utility>
#include <memory>
#include "GL/glew.h"
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

//        /** TODO remove
//         * Draw recursively the graph of the scene
//         * @param shaders
//         * @param pos_camera
//         * @param allow_debug
//         */
//        virtual void draw(Shaders *shaders, glm::vec3 pos_camera, bool allow_debug);

//            /**
//             * Get a Component
//             * @tparam T
//             * @return component or nullptr if not found
//             */
//            template<class T>
//            std::shared_ptr<T> get_component() {
//                ComponentType component_type = Component::get_type<T>();
//                if (m_components.find(component_type) == m_components.end()) {
//                    return nullptr;
//                } else {
////                    return m_components[component_type][0];
//                    return std::static_pointer_cast<T>(m_components[component_type][0]);
//                }
//            }
//
//            /**
//             * Get many Component
//             * @tparam T
//             * @return components
//             */
//            template<class T>
//            std::vector<std::shared_ptr<T>> get_components() {
//                ComponentType component_type = Component::get_type<T>();
//                if (m_components.find(component_type) == m_components.end()) {
//                    return {};
//                } else {
//                    int nb_components = (int)m_components[component_type].size();
//                    std::vector<std::shared_ptr<T>> components = {};
//                    components.resize(nb_components);
//                    for (int i = 0; i < nb_components; ++i) {
//                        components[i] = std::static_pointer_cast<T>(m_components[component_type][i]);
//                    }
////                    (m_components[component_type].begin(), m_components[component_type].end());
//                    return components;
////                    return m_components[component_type];
//                }
//            }
//
//            /**
//             * Add a Component to this node
//             * @tparam T
//             * @param component
//             */
//            template<class T>
//            void add_component(std::shared_ptr<T> component) {
//                ComponentType component_type = Component::get_type(*component);
//                auto component_cast = std::static_pointer_cast<Component>(component);
//                if(m_components.find(component_type) == m_components.end()){
//                    m_components[component_type] = {component_cast};
//                } else {
//                    m_components[component_type].push_back(component_cast);
//                }
//            }


            /// Destructor of the AbstractNode
            virtual ~AbstractNode();
        };
    }
}

#endif //PHOTONEAR_ABSTRACTNODE_H