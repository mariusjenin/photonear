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

typedef std::type_index ComponentType;

using namespace scene::node;

namespace component {
    /// Component addable to a Node in the SceneGraph Graph
    class Component {
    protected:

        Component() = default;
    public:
        static std::map<ComponentType,std::vector<std::pair<std::shared_ptr<Component>,std::shared_ptr<AbstractNode>>>> COMPONENTS;

        virtual ComponentType get_type() =0;

        template<class T>
        static ComponentType get_type() {
            return typeid(T*);
        }

        /**
         * Remove a Component from a node
         * @param component
         * @param node
         */
        static void remove_component_from_node(Component* component, AbstractNode* node = nullptr);


        /**
         * Add a Component to a node
         * @tparam T
         * @param component
         * @param node
         */
        template<class T>
        static void add_component_to_node(std::shared_ptr<Component> component, std::shared_ptr<AbstractNode> node){
            auto component_type = Component::get_type<T>();
            auto iter = COMPONENTS.find(component_type);
            if (iter != COMPONENTS.end() )
            {
                COMPONENTS.insert({component_type, {}});
            }

            auto comp = std::static_pointer_cast<T>(component);
            for(auto pair: COMPONENTS[component_type]){
                if(pair.first == comp && pair.second == node){
                    return;
                }
            }
            COMPONENTS[component_type].push_back(std::pair<std::shared_ptr<T>, std::shared_ptr<AbstractNode>>(comp, node));
        }

        /**
         * Get a Component (can be from a specific Node)
         * @tparam T
         * @param node
         * @return component or nullptr if not found
         */
        template<class T>
        static std::shared_ptr<T> get_component(AbstractNode* node = nullptr) {
            auto component_type = Component::get_type<T>();
            if (COMPONENTS.find(component_type) == COMPONENTS.end()) {
                return nullptr;
            } else {
                int nb_components = (int)COMPONENTS[component_type].size();
                std::shared_ptr<T> component;
                for (int i = 0; i < nb_components; ++i) {
                    if(&*(COMPONENTS[component_type][i].second) == node || node == nullptr){
                        return std::static_pointer_cast<T>(COMPONENTS[component_type][i].first);
                    }
                }
                return nullptr;
            }
        }

        /**
         * Get a list of Component (can be from a specific Node)
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
                int nb_components = (int)COMPONENTS[component_type].size();
                std::vector<std::shared_ptr<T>> components = {};
                for (int i = 0; i < nb_components; ++i) {
                    if(&*(COMPONENTS[component_type][i].second) == node || node == nullptr) {
                        components.push_back(std::static_pointer_cast<T>(COMPONENTS[component_type][i].first));
                    }
                }
                return components;
            }
        }

        /**
         * Get a list of Component from children of a Node
         * @tparam T
         * @param node
         * @return components
         */
        template<class T>
        static std::vector<std::shared_ptr<T>> get_children_components(AbstractNode* node) {
            auto children = node->get_children();
            if(children.empty()){
                return {};
            }
            std::vector<std::shared_ptr<T>> components = {};
            for(const auto& child_node : children){
                auto child_components = get_components<T>((AbstractNode*)&*child_node);
                components.insert(components.end(),child_components.begin(),child_components.end());
            }
            return components;
        }
        /**
         * Get a Component from parents recursively
         * @tparam T
         * @param node
         * @return component
         */
        template<class T>
        static std::shared_ptr<T> get_parent_component_recursive(AbstractNode* node) {
            auto parent = node->get_parent();
            if(parent == nullptr){
                return nullptr;
            }
            auto component = get_component<T>(&*parent);
            if(component == nullptr){
                component = get_parent_component_recursive<T>(&*parent);
            }
            return component;
        }

        /**
         * Get the Node that has the Component
         * @param component
         * @return node or nullptr if not found
         */
        static std::shared_ptr<AbstractNode> get_node(Component* component);

    };
}

#endif //PHOTONEAR_COMPONENT_H
