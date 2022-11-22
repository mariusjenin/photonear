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
    /// Component addable to a Node in the Scene Graph
    class Component {
    protected:

        Component() = default;
    public:
        static std::map<ComponentType,std::vector<std::pair<std::shared_ptr<Component>,std::shared_ptr<AbstractNode>>>> COMPONENTS;

        virtual ComponentType get_type()=0;

        template<class T>
        static ComponentType get_type() {
            return typeid(T*);
        }

        /**
         * Remove a Component from a node
         * @param component
         * @param node
         */
        static void remove_component_from_node(std::shared_ptr<Component> component, const std::shared_ptr<AbstractNode>& node = nullptr);


        /**
         * Add a Component to a node
         * @tparam T
         * @param component
         * @param node
         */
        template<class T>
        static void add_component_to_node(std::shared_ptr<Component> component, std::shared_ptr<AbstractNode> node){
            auto type_comp = Component::get_type<T>();
            auto iter = COMPONENTS.find(type_comp);
            if (iter != COMPONENTS.end() )
            {
                COMPONENTS.insert({type_comp, {}});
            }

            auto comp = std::static_pointer_cast<T>(component);
            for(auto pair: COMPONENTS[type_comp]){
                if(pair.first == comp && pair.second == node){
                    return;
                }
            }
            COMPONENTS[type_comp].push_back(std::pair<std::shared_ptr<T>, std::shared_ptr<AbstractNode>>(std::static_pointer_cast<T>(component), node));
        }

        /**
         * Get a Component
         * @tparam T
         * @param node
         * @return component or nullptr if not found
         */
        template<class T>
        static std::shared_ptr<T> get_component(const std::shared_ptr<AbstractNode>& node = nullptr) {
            ComponentType component_type = Component::get_type<T>();
            if (COMPONENTS.find(component_type) == COMPONENTS.end()) {
                return nullptr;
            } else {
                int nb_components = (int)COMPONENTS[component_type].size();
                std::shared_ptr<T> component;
                for (int i = 0; i < nb_components; ++i) {
                    if(COMPONENTS[component_type][i].second == node || node == nullptr){
                        return std::static_pointer_cast<T>(COMPONENTS[component_type][i].first);
                    }
                }
                return nullptr;
            }
        }

        /**
         * Get a list of Component
         * @tparam T
         * @param node
         * @return component or nullptr if not found
         */
        template<class T>
        static std::vector<std::shared_ptr<T>> get_components(const std::shared_ptr<AbstractNode>& node = nullptr) {
            ComponentType component_type = Component::get_type<T>();
            if (COMPONENTS.find(component_type) == COMPONENTS.end()) {
                return {};
            } else {
                int nb_components = (int)COMPONENTS[component_type].size();
                std::vector<std::shared_ptr<T>> components = {};
                for (int i = 0; i < nb_components; ++i) {
                    if(COMPONENTS[component_type][i].second == node || node == nullptr) {
                        components.push_back(std::static_pointer_cast<T>(COMPONENTS[component_type][i].first));
                    }
                }
                return components;
            }
        }

        /**
         * Get the Node that has the Component
         * @param component
         * @return node or nullptr if not found
         */
        static std::shared_ptr<AbstractNode> get_node(const std::shared_ptr<Component>& component);

    };
}

#endif //PHOTONEAR_COMPONENT_H
