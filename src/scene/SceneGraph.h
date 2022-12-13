//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SCENEGRAPH_H
#define PHOTONEAR_SCENEGRAPH_H

#include "memory"
#include "RootNode.h"
#include "BoundingBox.h"
#include "RayCastHit.h"

using namespace scene::node;

namespace scene {
    enum ShapeInHierarchy{
        NoShape,
        InNode,
        InChildren
    };
    class SceneGraph {
    private:
        std::shared_ptr<RootNode> m_root_node;
        /**
         * Generate BoundingBox Component to a node recursively if it has more than 1 Shape or 1 Shape and 1 Shape in one of his branch
         * @param node
         * @return shape_in_hierarchy
         */
        ShapeInHierarchy generate_bounding_boxes_recursive(const std::shared_ptr<AbstractNode>& node);

        BoundingBox* compute_bounding_boxes_recursive(AbstractNode* node);

    public:
        explicit SceneGraph(std::shared_ptr<RootNode> rn);

        std::shared_ptr<RootNode> get_root_node();

        void generate_ui_scene_graph();

        void generate_bounding_boxes();

        void compute_scene_graph();

        void compute_bounding_boxes();

        std::shared_ptr<RayCastHit> raycast(Ray ray, float refraction_index_medium);

        void raycast_recursive(Ray ray, const std::shared_ptr<AbstractNode> &node, ray_tracing::RayCastHit *pHit);
    };
}

#endif //PHOTONEAR_SCENEGRAPH_H
