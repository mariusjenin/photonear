//
// Created by mariusjenin on 19/11/22.
//

#include "algorithm"
#include "AbstractNode.h"
#include "Component.h"

using namespace component;
using namespace scene::node;

std::map<ComponentType, std::vector<std::pair<std::shared_ptr<Component>, std::shared_ptr<AbstractNode>>>> Component::COMPONENTS = {};


void Component::add_component_to_node(const std::shared_ptr<Component>& component, const std::shared_ptr<AbstractNode>& node) {
    auto component_type = component->get_type();
    auto iter = COMPONENTS.find(component_type);
    if (iter != COMPONENTS.end() )
    {
        COMPONENTS.insert({component_type, {}});
    }

    for(const auto& pair: COMPONENTS[component_type]){
        if(pair.first == component && pair.second == node){
            return;
        }
    }
    COMPONENTS[component_type].push_back(std::pair<std::shared_ptr<Component>, std::shared_ptr<AbstractNode>>(component, node));
}

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

void Component::clear_components() {
    COMPONENTS.clear();
}
