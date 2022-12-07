//
// Created by mariusjenin on 27/11/22.
//

#ifndef PHOTONEAR_QUADSCENE_H
#define PHOTONEAR_QUADSCENE_H

#include "CornellBox.h"
#include "Scene.h"

namespace scene {
    class QuadScene : public Scene {
    public:
        QuadScene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path,
                  vec3 clear_color = {0, 0, 0});
    private:
        void init_scene_graph() override;
    };
}

#endif //PHOTONEAR_QUADSCENE_H
