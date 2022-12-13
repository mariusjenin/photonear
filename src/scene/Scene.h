//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SCENE_H
#define PHOTONEAR_SCENE_H

#include "memory"
#include "SceneGraph.h"
#include "VertFragShaders.h"
#include "EmissiveMaterial.h"
#include "Material.h"
#include "Shape.h"
#include "Camera.h"

using namespace shader_manager;
using namespace component;
using namespace component::material;

namespace scene {
    class Scene {
    private:
        int m_nb_lights;

        void load_lights();

        void load_camera();

        static void draw_shapes(const std::shared_ptr<Shaders>& shaders) ;

        void generate_texture();

        void resize_texture() const;

        virtual void init_scene_graph() = 0;

    protected:
        static const int NB_MAX_LIGHTS = 10;
        GLFWwindow *m_window;
        std::shared_ptr<VertFragShaders> m_shaders;
        std::shared_ptr<SceneGraph> m_scene_graph;

        GLuint m_frame_buffer{};
        GLuint m_render_buffer{};
        GLuint m_texture{};

        int m_width_viewer{};
        int m_height_viewer{};

        color m_default_color{};
        color m_debug_color{};
        color m_debug_color_2{};

        bool m_debug_enabled{};
        int m_debug_depth{};

        bool m_auto_draw{};
        bool m_camera_valid{};
        bool m_scene_valid{};
        bool m_viewer_valid{};


        Scene(GLFWwindow *window, const std::string &vertex_shader_path, const std::string &fragment_shader_path);
    public:
        void init();

        void update(float delta_time);

        void draw(bool force = false);

        void draw_debug();

        /**
         * Process the input of the user to have actions on the Scene
         * @param delta_time
         */
        void handle_inputs(float delta_time);

        void init_camera();

        static std::vector<EmissiveMaterial*> get_lights();

        static std::shared_ptr<Camera> get_active_camera();

        void set_viewer_size(int width, int height);

        void set_scene_valid(bool valid = false);

        void set_viewer_valid(bool valid = false);

        std::shared_ptr<SceneGraph> get_scene_graph();

        void generate_ui_scene_settings();

        void generate_ui_viewer() const;

        int get_nb_lights() const;
    };
}
#endif //PHOTONEAR_SCENE_H
