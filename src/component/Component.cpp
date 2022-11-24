//
// Created by mariusjenin on 19/11/22.
//

#include "algorithm"
#include "AbstractNode.h"
#include "Component.h"

using namespace component;
using namespace scene::node;

std::map<ComponentType, std::vector<std::pair<std::shared_ptr<Component>, std::shared_ptr<AbstractNode>>>> Component::COMPONENTS = {};

void Component::remove_component_from_node(Component* component, AbstractNode* node) {
    ComponentType component_type = component->get_type();
    int nb_components = (int)COMPONENTS[component_type].size();
    for (int i = 0; i < nb_components; ++i) {
        auto pair = COMPONENTS[component_type][i];
        if(&*pair.first == component && (node == nullptr || &*pair.second == node)){
            COMPONENTS[component_type].erase(COMPONENTS[component_type].begin()+i);
        }
    }
}


std::shared_ptr<AbstractNode> Component::get_node(Component* component) {
    ComponentType component_type = component->get_type();
    if (COMPONENTS.find(component_type) == COMPONENTS.end()) {
        return nullptr;
    } else {
        int nb_components = (int) COMPONENTS[component_type].size();
        for (int i = 0; i < nb_components; ++i) {
            if (&*(COMPONENTS[component_type][i].first) == component) {
                return std::static_pointer_cast<AbstractNode>(COMPONENTS[component_type][i].second);
            }
        }
        return nullptr;
    }
}