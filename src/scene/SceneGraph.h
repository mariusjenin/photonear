//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SCENEGRAPH_H
#define PHOTONEAR_SCENEGRAPH_H

#include "memory"
#include "RootNode.h"

using namespace scene::node;

namespace scene {
    class SceneGraph {
    private:
        std::shared_ptr<RootNode> m_root_node;

    public:
        explicit SceneGraph(std::shared_ptr<RootNode> rn);

        void generate_scene_graph_ui();

        void compute_scene_graph();
    };
}

#endif //PHOTONEAR_SCENEGRAPH_H
