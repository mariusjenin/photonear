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

using namespace scene;


class Photonear {
private:
    static const char * SceneGraphEditorName;
    static const char * NodeEditorName;
    static const char * ComponentEditorName;
    static const char * OpenGLViewerName;
    static const char * PhotonMappingViewerName;
    static const char * SceneSettingsName;
    static const char * PhotonMappingSettingsName;
    static const char * LogsName;

    static Photonear* PhotonearInstance;
    std::shared_ptr<Scene> m_scene;
    GLFWwindow* m_window{};
    bool m_init_ui{};
    AbstractNode* m_node_selected{};
    Component* m_component_selected{};

    Photonear() = default;

    static void dock_ui();

    void reinit_ui();

    void generate_ui_node_editor();

    void generate_ui_component_editor();

    static void window_resize_callback(GLFWwindow* window, int width,int height);
public:
    static Photonear* get_instance();

    void set_window(GLFWwindow* window);

    void init();

    void draw(float delta_time);

    void finish();

    void set_node_selected(AbstractNode* node_selected);

    AbstractNode* get_node_selected();

    void set_component_selected(Component* component_selected);

    Component* get_component_selected();
};
#endif //PHOTONEAR_PHOTONEAR_H
