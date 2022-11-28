//
// Created by mariusjenin on 19/11/22.
//

#include "algorithm"
#include "imgui.h"

#include "AbstractNode.h"
#include "Component.h"
#include "Photonear.h"

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

std::vector<std::shared_ptr<Component>> Component::get_components(AbstractNode *node){
    std::vector<std::shared_ptr<Component>> components = {};
    for(const auto& pair : COMPONENTS){
        auto list_component = pair.second;
        int nb_components = (int) list_component.size();
        for (int i = 0; i < nb_components; ++i) {
            if (&*(list_component[i].second) == node) {
                components.push_back(list_component[i].first);
            }
        }
    }
    return components;

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

std::string Component::get_ui_name(){
    std::string name_with_package = abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, nullptr);
    std::string sep = "::";
    return name_with_package.substr(name_with_package.find_last_of(sep)+1,name_with_package.size());
}

void Component::generate_node_editor_ui() {
    auto photonear = Photonear::get_instance();
    auto component_selected = photonear->get_component_selected();
    ImGui::PushID(this);
    if (ImGui::Selectable(get_ui_name().c_str(),component_selected == this)){
        photonear->set_component_selected(this);
    }
    ImGui::PopID();
}
