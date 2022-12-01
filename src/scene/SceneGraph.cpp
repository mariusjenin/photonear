//
// Created by mariusjenin on 21/11/22.
//

#include <utility>
#include <list>

#include "SceneGraph.h"

#include "Node.h"
#include "Component.h"
#include "TransformComponent.h"
#include "Shape.h"

using namespace scene;
using namespace component;
using namespace component::shape;
using namespace scene::node;

SceneGraph::SceneGraph(std::shared_ptr<RootNode> rn) {
    m_root_node = std::move(rn);
    auto trsf_comp_root = std::make_shared<TransformComponent>();
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
    generate_bounding_boxes_recursively(m_root_node);
}

ShapeInHierarchy SceneGraph::generate_bounding_boxes_recursively(const std::shared_ptr<AbstractNode> &node) {
    if (Component::get_component<Shape>(&*node) != nullptr) {
        Component::add_component_to_node(std::make_shared<BoundingBox>(), node);
        return ShapeInHierarchy::InNode;
    } else {
        std::vector<std::shared_ptr<Node>> children = node->get_children();
        int count_child_shape = 0;
        int count_shape_in_hierarchy = 0;
        for (const auto &child: children) {
            auto shape_in_hierarchy = generate_bounding_boxes_recursively(child);
            if (shape_in_hierarchy == ShapeInHierarchy::InChildren) count_shape_in_hierarchy++;
            if (shape_in_hierarchy == ShapeInHierarchy::InNode) count_child_shape++;
        }
        if (count_child_shape > 1 || (count_child_shape == 1 && count_shape_in_hierarchy > 0)) {
            Component::add_component_to_node(std::make_shared<BoundingBox>(), node);
            return ShapeInHierarchy::InChildren;
        }
        return ShapeInHierarchy::NoShape;
    }
}

void SceneGraph::compute_bounding_boxes() {
    compute_bounding_boxes_recursively(&*m_root_node);
}


BoundingBox* SceneGraph::compute_bounding_boxes_recursively(AbstractNode *node) {
    BoundingBox* bounding_box = &*Component::get_component<BoundingBox>(&*node);
    if(bounding_box == nullptr){
        bounding_box = new BoundingBox();
    } else {
        bounding_box->init();
    }

    auto shapes = Component::get_components<Shape>(node);
    for (const auto& shape: shapes) {
        auto vertices = shape->to_few_vertices();
        auto shape_node = Component::get_node(&*shape);
        auto trsf_comp = Component::get_component<TransformComponent>(&*shape_node);
        auto matrix = trsf_comp->get_matrix_as_end_node();
        for(int i = 0 ; i < 8 ; i ++){
            vertices[i] = matrix*vec4(vertices[i],1);
        }

        bounding_box->merge(vertices);
    }

    auto children = node->get_children();
    std::vector<std::shared_ptr<BoundingBox>> child_bbs = {};
    for (const auto &child: children) {
        auto child_bb = compute_bounding_boxes_recursively(&*child);
        if (child_bb != nullptr) {
            bounding_box->merge(*child_bb);
        }
    }
    return bounding_box;
}