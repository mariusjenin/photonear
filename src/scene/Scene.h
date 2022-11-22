//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SCENE_H
#define PHOTONEAR_SCENE_H

#include "memory"
#include "RootNode.h"

using namespace scene::node;

namespace scene {
    class Scene {
    private:
        std::shared_ptr<RootNode> m_root_node;

    public:
        explicit Scene(std::shared_ptr<RootNode> rn);

        std::shared_ptr<RootNode> get_root_node();
    };
}

#endif //PHOTONEAR_SCENE_H
