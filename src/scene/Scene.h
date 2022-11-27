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

        void load_lights();

        static void draw_shapes(const std::shared_ptr<Shaders>& shaders);

        void generate_texture();

        void resize_texture() const;

        virtual void init_scene_graph() = 0;
    protected:
        static const int NB_MAX_LIGHTS = 10;
        GLFWwindow *m_window;
        GLuint m_frame_buffer{};
        GLuint m_render_buffer{};
        GLuint m_texture{};
        int m_width_viewer{};
        int m_height_viewer{};
        vec3 m_clear_color{};
        std::shared_ptr<VertFragShaders> m_shaders;
        std::shared_ptr<SceneGraph> m_scene_graph;

        Scene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path,
              vec3 clear_color = {0, 0, 0});
    public:
        void init();

        void update(float delta_time);

        void draw();

        /**
         * Process the input of the user to have actions on the Scene
         * @param delta_time
         */
        void handle_inputs(float delta_time);

        GLuint get_texture() const;

        void set_viewer_size(int width, int height);
    };
}
#endif //PHOTONEAR_SCENE_H
