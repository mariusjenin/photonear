//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_COMPONENT_H
#define PHOTONEAR_COMPONENT_H

#include <cxxabi.h>
#include <typeinfo>
#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>

#include <memory>
#include <map>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include "AbstractNode.h"
#include "Shaders.h"

typedef std::type_index ComponentType;

using namespace scene::node;
using namespace shader_manager;

namespace component {
    /// Component addable to a Node in the SceneGraph Graph
    class Component {
    private:
    protected:
        static std::map<ComponentType, std::vector<std::pair<std::shared_ptr<Component>, std::shared_ptr<AbstractNode>>>> COMPONENTS;

        Component() = default;

    public:

        virtual ComponentType get_type() = 0;

        virtual void generate_ui_node_editor_ui();

        virtual void generate_ui_component_editor() = 0;

        std::string get_ui_name();

        virtual void draw(const std::shared_ptr<Shaders> &shaders, color color){};

        template<class T>
        static ComponentType get_type() {
            return typeid(T *);
        }

        /**
         * Clear the Component ECS structure
         */
        static void clear_components();

        /**
         * Remove a Component from a node
         * @param component
         * @param node
         */
        static void remove_component_from_node(Component *component, AbstractNode *node = nullptr);


        /**
         * Add a Component to a node
         * @param component
         * @param node
         */
        static void
        add_component_to_node(const std::shared_ptr<Component> &component, const std::shared_ptr<AbstractNode> &node);

        /**
         * Get a Component (can be from a specific Node)
         * @tparam T
         * @param node
         * @return component or nullptr if not found
         */
        template<class T>
        static std::shared_ptr<T> get_component(AbstractNode *node = nullptr) {
            auto component_type = Component::get_type<T>();
            if (COMPONENTS.find(component_type) == COMPONENTS.end()) {
                return nullptr;
            } else {
                int nb_components = (int) COMPONENTS[component_type].size();
                std::shared_ptr<T> component;
                for (int i = 0; i < nb_components; ++i) {
                    if (&*(COMPONENTS[component_type][i].second) == node || node == nullptr) {
                        return std::static_pointer_cast<T>(COMPONENTS[component_type][i].first);
                    }
                }
                return nullptr;
            }
        }

        /**
         * Get a list of Component of a specific type (can be from a specific Node)
         * @tparam T
         * @param node
         * @return components
         */
        template<class T>
        static std::vector<std::shared_ptr<T>> get_components(AbstractNode *node = nullptr) {
            auto component_type = Component::get_type<T>();
            if (COMPONENTS.find(component_type) == COMPONENTS.end()) {
                return {};
            } else {
                int nb_components = (int) COMPONENTS[component_type].size();
                std::vector<std::shared_ptr<T>> components = {};
                for (int i = 0; i < nb_components; ++i) {
                    if (&*(COMPONENTS[component_type][i].second) == node || node == nullptr) {
                        components.push_back(std::static_pointer_cast<T>(COMPONENTS[component_type][i].first));
                    }
                }
                return components;
            }
        }

        /**
         * Get all the Component of a Node
         * @param node
         * @return components
         */
        static std::vector<std::shared_ptr<Component>> get_components(AbstractNode *node);

        /**
         * Get a list of Component from children of a Node
         * @tparam T
         * @param node
         * @param recursive
         * @return components
         */
        template<class T>
        static std::vector<std::shared_ptr<T>> get_children_components(AbstractNode *node, bool recursive = false) {
            auto children = node->get_children();
            if (children.empty()) {
                if(recursive){
                    return get_components<T>((AbstractNode *) &*node);
                } else {
                    return {};
                }
            }
            std::vector<std::shared_ptr<T>> components = {};
            for (const auto &child_node: children) {
                std::vector<std::shared_ptr<T>> child_components;
                if(recursive){
                    child_components = get_children_components<T>((AbstractNode *) &*child_node, true);
                } else {
                    child_components = get_components<T>((AbstractNode *) &*child_node);
                }
                components.insert(components.end(), child_components.begin(), child_components.end());
            }
            return components;
        }

        /**
         * Get nearest Component recursively upper in the hierarchy
         * @tparam T
         * @param node
         * @return component
         */
        template<class T>
        static std::shared_ptr<T> get_nearest_component_upper(AbstractNode *node) {
            if (node == nullptr) {
                return nullptr;
            }
            auto component = get_component<T>(&*node);
            if (component == nullptr) {
                component = get_nearest_component_upper<T>(&*node->get_parent());
            }
            return component;
        }

        /**
         * Get the Node that has a Component of the type specified
         * @tparam T
         * @param component
         * @return node or nullptr if not found
         */
        template<class T>
        static std::shared_ptr<AbstractNode> get_node_having() {
            ComponentType component_type = get_type<T>();
            if (COMPONENTS.find(component_type) == COMPONENTS.end()) {
                return nullptr;
            } else {
                return std::static_pointer_cast<AbstractNode>(COMPONENTS[component_type][0].second);
            }
        }


        /**
         * Get the Node that has the Component
         * @param component
         * @return node or nullptr if not found
         */
        static std::shared_ptr<AbstractNode> get_node(Component *component);
    };
}

#endif //PHOTONEAR_COMPONENT_H
