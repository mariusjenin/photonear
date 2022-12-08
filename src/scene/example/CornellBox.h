//
// Created by mariusjenin on 27/11/22.
//

#ifndef PHOTONEAR_CORNELLBOX_H
#define PHOTONEAR_CORNELLBOX_H

#include "Scene.h"

namespace scene {
    class CornellBox : public Scene {
    public:
        CornellBox(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path);
    private:
        void init_scene_graph() override;
    };
}

#endif //PHOTONEAR_CORNELLBOX_H
