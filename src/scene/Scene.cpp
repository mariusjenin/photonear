//
// Created by mariusjenin on 21/11/22.
//

#include "Scene.h"

using namespace scene;

Scene::Scene(std::shared_ptr<RootNode> rn) {
    m_root_node = rn;
}

std::shared_ptr<RootNode> Scene::get_root_node() {
    return m_root_node;
}