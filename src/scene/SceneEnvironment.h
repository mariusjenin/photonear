//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SCENEENVIRONMENT_H
#define PHOTONEAR_SCENEENVIRONMENT_H

#include "memory"
#include "Scene.h"
#include "VertFragShaders.h"
#include "Material.h"
#include "Hittable.h"

using namespace shader_manager;
using namespace component;

namespace scene {
    class SceneEnvironment {
    private:
        GLFWwindow *m_window;
        std::shared_ptr<VertFragShaders> m_shaders;
        std::shared_ptr<Scene> m_scene;
        float m_fovy;
        float m_z_near;
        float m_z_far;

        void load_projection_matrix();

        void load_view_mat_pos();

    public:
        SceneEnvironment(GLFWwindow *window,
                         const std::string &vertex_shader_path, const std::string &fragment_shader_path,
                         std::shared_ptr<Scene> scene, vec3 clear_color = {0,0,0},
                         float fovy = 45.0f, float z_near = 0.1f, float z_far = 10000.0f);

        void render();

        void draw_node_recursive(const std::shared_ptr<Material>& material, const std::shared_ptr<AbstractNode>& node);

        void draw_shape(std::shared_ptr<Material> material, const std::shared_ptr<Hittable>& hittable);
    };
}
#endif //PHOTONEAR_SCENEENVIRONMENT_H
