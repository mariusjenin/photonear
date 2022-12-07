//
// Created by mariusjenin on 21/11/22.
//

#include <utility>
#include <list>

#include "SceneGraph.h"

#include "Node.h"
#include "Component.h"
#include "RootTransformComponent.h"
#include "Shape.h"
#include "Photonear.h"

using namespace scene;
using namespace component;
using namespace component::shape;
using namespace scene::node;

SceneGraph::SceneGraph(std::shared_ptr<RootNode> rn) {
    m_root_node = std::move(rn);
    auto trsf_comp_root = std::make_shared<RootTransformComponent>();
    m_root_node->set_trsf_comp(trsf_comp_root);
    Component::add_component_to_node(trsf_comp_root, m_root_node);
}

void SceneGraph::compute_scene_graph() {
    auto trsf_comp = m_root_node->get_trsf_comp();
    trsf_comp->compute_trsf_scene_graph();
}

void SceneGraph::generate_ui_scene_graph() {
    m_root_node->generate_ui_scene_graph();
}

void SceneGraph::generate_bounding_boxes() {
    generate_bounding_boxes_recursive(m_root_node);
}

ShapeInHierarchy SceneGraph::generate_bounding_boxes_recursive(const std::shared_ptr<AbstractNode> &node) {
    ShapeInHierarchy shape_in_hierarchy = ShapeInHierarchy::NoShape;
    bool added = false;
    if (Component::get_component<Shape>(&*node) != nullptr) {
        Component::add_component_to_node(std::make_shared<BoundingBox>(), node);
        added = true;
        shape_in_hierarchy = ShapeInHierarchy::InNode;
    }

    std::vector<std::shared_ptr<Node>> children = node->get_children();
    int count_child_shape = 0;
    int count_shape_in_hierarchy = 0;
    for (const auto &child: children) {
        auto shape_in_hierarchy_child = generate_bounding_boxes_recursive(child);
        if (shape_in_hierarchy_child == ShapeInHierarchy::InChildren) count_shape_in_hierarchy++;
        if (shape_in_hierarchy_child == ShapeInHierarchy::InNode) count_child_shape++;
    }
    if(!added){
        if (count_child_shape > 1 || count_shape_in_hierarchy > 1 || (count_child_shape > 0 && count_shape_in_hierarchy >0)) {
            Component::add_component_to_node(std::make_shared<BoundingBox>(), node);
            shape_in_hierarchy = ShapeInHierarchy::InChildren;
        }
    }
    return shape_in_hierarchy;
}

void SceneGraph::compute_bounding_boxes() {
    compute_bounding_boxes_recursive(&*m_root_node);
}


BoundingBox* SceneGraph::compute_bounding_boxes_recursive(AbstractNode *node) {
    BoundingBox* bounding_box = &*Component::get_component<BoundingBox>(&*node);
    bool init_state = false;
    if(bounding_box == nullptr){
        bounding_box = new BoundingBox();
        init_state = true;
    } else {
        bounding_box->init();
    }

    auto trsf_comp = Component::get_component<TransformComponent>(&*node);
    auto matrix = trsf_comp->get_matrix_as_end_node();
    auto shapes = Component::get_components<Shape>(node);
    for (const auto& shape: shapes) {
        auto vertices = shape->to_few_vertices();
        for(int i = 0 ; i < 8 ; i ++){
            vertices[i] = matrix*vec4(vertices[i],1);
        }
        bounding_box->merge(vertices);
    }

    auto children = node->get_children();
    std::vector<std::shared_ptr<BoundingBox>> child_bbs = {};
    for (const auto &child: children) {
        auto child_bb = compute_bounding_boxes_recursive(&*child);
        if (child_bb != nullptr) {
            bounding_box->merge(*child_bb);
            init_state = false;
        }
    }
    if(init_state) return nullptr;
    return bounding_box;
}

bool SceneGraph::raycast(Ray ray) {
    return raycast_recursive(ray, m_root_node);
}

bool SceneGraph::raycast_recursive(Ray ray, const std::shared_ptr<AbstractNode>& node) {
    if(!node->is_active_recursive()) return false;
    auto bb = Component::get_component<BoundingBox>(&*node);
    if(bb!=nullptr && !bb->hit_by_ray(ray)){
        return false;
    }

    auto shapes = Component::get_components<Shape>(&*node);
    for(const auto& shape:shapes){
        if(shape->hit(ray)){
            return true;
        }
    }

    bool hit = false;
    auto children = node->get_children();
    for(const auto& child:children){
        hit = hit || raycast_recursive(ray, child);
        if(hit) return true;
    }
    return hit;
}

std::shared_ptr<RootNode> SceneGraph::get_root_node() {
    return m_root_node;
}
