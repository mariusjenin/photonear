//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SCENE_H
#define PHOTONEAR_SCENE_H

#include "memory"
#include "SceneGraph.h"
#include "VertFragShaders.h"
#include "Material.h"
#include "Shape.h"

using namespace shader_manager;
using namespace component;

namespace scene {
    class Scene {
    private:
        GLFWwindow *m_window;
        std::shared_ptr<VertFragShaders> m_shaders;
        std::shared_ptr<SceneGraph> m_scene_graph;

//        void load_lights(); TODO

    public:
        Scene(GLFWwindow *window,
              const std::string &vertex_shader_path, const std::string &fragment_shader_path,
              std::shared_ptr<SceneGraph> scene, vec3 clear_color = {0, 0, 0});

        void draw();
    };
}
#endif //PHOTONEAR_SCENE_H
