//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SCENEENVIRONMENT_H
#define PHOTONEAR_SCENEENVIRONMENT_H

#include "memory"
#include "Scene.h"
#include "VertFragShaders.h"
#include "Material.h"
#include "Shape.h"

using namespace shader_manager;
using namespace component;

namespace scene {
    class SceneEnvironment {
    private:
        GLFWwindow *m_window;
        std::shared_ptr<VertFragShaders> m_shaders;
        std::shared_ptr<Scene> m_scene;

//        void load_lights(); TODO

    public:
        SceneEnvironment(GLFWwindow *window,
                         const std::string &vertex_shader_path, const std::string &fragment_shader_path,
                         std::shared_ptr<Scene> scene, vec3 clear_color = {0,0,0});

        void draw();
    };
}
#endif //PHOTONEAR_SCENEENVIRONMENT_H
