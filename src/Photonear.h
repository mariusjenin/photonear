//
// Created by mariusjenin on 27/11/22.
//

#ifndef PHOTONEAR_PHOTONEAR_H
#define PHOTONEAR_PHOTONEAR_H

#include "Scene.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "RayTracer.h"
#include "PhotonMapper.h"

using namespace scene;
using namespace ray_tracing;

class Photonear {
private:
    static Photonear* PhotonearInstance;

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<RayTracer> m_ray_tracer;
    std::shared_ptr<PhotonMapper> m_photon_mapper;

    GLFWwindow* m_window{};

    AbstractNode* m_node_selected{};
    Component* m_component_selected{};

    bool m_init_docking{};

    Photonear() = default;

    static void dock_ui();

    void init_docking();

    void generate_ui_node_editor();

    void generate_ui_component_editor();

    static void window_resize_callback(GLFWwindow* window, int width,int height);
public:
    static const char * SceneGraphEditorName;
    static const char * NodeEditorName;
    static const char * ComponentEditorName;
    static const char * OpenGLViewerName;
    static const char * PhotonMappingViewerName;
    static const char * SceneSettingsName;
    static const char * RayTracingSettingsName;
    static const char * PhotonMappingSettingsName;
    static const char * LogsName;

    static Photonear* get_instance();

    void set_window(GLFWwindow* window);

    void init();

    void update(float delta_time);

    void start_frame();

    void finish_frame();

    void draw();

    void finish();

    void set_node_selected(AbstractNode* node_selected);

    std::shared_ptr<RayTracer> get_ray_tracer();

    std::shared_ptr<PhotonMapper> get_photon_mapper();

    std::shared_ptr<Scene> get_scene();

    AbstractNode* get_node_selected();

    Component* get_component_selected();

    void set_component_selected(Component* component_selected);
};
#endif //PHOTONEAR_PHOTONEAR_H
