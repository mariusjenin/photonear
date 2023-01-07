//
// Created by mariusjenin on 27/11/22.
//

#include "Photonear.h"
#include "QuadScene.h"
#include "CornellBox.h"
#include "PhotonMapTestScene.h"

Photonear *Photonear::PhotonearInstance = nullptr;

const char *Photonear::SceneGraphEditorName = "Scene Graph Editor";
const char *Photonear::NodeEditorName = "Node Editor";
const char *Photonear::ComponentEditorName = "Component Editor";
const char *Photonear::OpenGLViewerName = "OpenGL Viewer";
const char *Photonear::PhotonMappingViewerName = "Photon Mapping Viewer";
const char *Photonear::SceneSettingsName = "Scene Settings";
const char *Photonear::RayTracingSettingsName = "Ray Tracing Settings";
const char *Photonear::PhotonMappingSettingsName = "Photon Mapping Settings";
const char *Photonear::LogsName = "Logs";

Photonear *Photonear::get_instance() {
    if (PhotonearInstance == nullptr) {
        PhotonearInstance = new Photonear();
    }
    return PhotonearInstance;
}

void Photonear::init() {
    m_init_docking = true;
    m_scene = std::make_shared<CornellBox>(m_window, "../src/shader/vertex_shader.glsl",
                                           "../src/shader/fragment_shader.glsl");

    //Callback resize
    glfwSetWindowSizeCallback(m_window, Photonear::window_resize_callback);

    // Dear ImGui settings
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Enable Docking and Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();

    m_ray_tracer = std::make_shared<RayTracer>();
    m_ray_tracer->init();

    m_photon_mapper = std::make_shared<PhotonMapper>();
    m_photon_mapper->init();

    m_scene->init();

    m_scene->set_viewer_valid(false);
    m_node_selected = &*m_scene->get_scene_graph()->get_root_node();
}

void Photonear::init_docking() {
    m_init_docking = true;
}

void Photonear::update(float delta_time) {
    if (m_init_docking) {
        dock_ui();
        m_init_docking = false;
    }

    ImGui::Begin(OpenGLViewerName, nullptr, ImGuiWindowFlags_NoMove);
    auto window_size = ImGui::GetWindowSize();
    ImGui::End();

    m_scene->set_viewer_size((int) window_size.x, (int) window_size.y);
    m_scene->update(delta_time);

    m_ray_tracer->update();
    m_photon_mapper->update();
}

void Photonear::draw() {
    // Scene Graph
    ImGui::Begin(SceneGraphEditorName, nullptr, ImGuiWindowFlags_NoMove);
    m_scene->get_scene_graph()->generate_ui_scene_graph();
    ImGui::End();

    // Node Editor
    ImGui::Begin(NodeEditorName, nullptr, ImGuiWindowFlags_NoMove);
    generate_ui_node_editor();
    ImGui::End();

    // Component Editor
    ImGui::Begin(ComponentEditorName, nullptr, ImGuiWindowFlags_NoMove);
    generate_ui_component_editor();
    ImGui::End();

    //OpenGL Viewer
    ImGui::Begin(OpenGLViewerName, nullptr, ImGuiWindowFlags_NoMove);
    m_scene->draw();
    m_scene->generate_ui_viewer();
    ImGui::End();

    // Scene Settings
    ImGui::Begin(SceneSettingsName, nullptr, ImGuiWindowFlags_NoMove);
    m_scene->generate_ui_scene_settings();
    ImGui::End();

    // Ray Tracing Settings
    ImGui::Begin(RayTracingSettingsName, nullptr, ImGuiWindowFlags_NoMove);
    m_ray_tracer->generate_ui_ray_tracing_settings();
    ImGui::End();

    // Photon Mapping Settings
    ImGui::Begin(PhotonMappingSettingsName, nullptr, ImGuiWindowFlags_NoMove);
    m_photon_mapper->generate_ui_photon_mapping_settings();
    ImGui::Separator();
    m_ray_tracer->generate_ui_photon_gathering_settings();
    ImGui::End();

    // Photon Mapping Viewer
    ImGui::Begin(PhotonMappingViewerName, nullptr, ImGuiWindowFlags_NoMove);
    m_ray_tracer->generate_ui_viewer();
    ImGui::End();

    // Logs
    ImGui::Begin(LogsName, nullptr, ImGuiWindowFlags_NoMove);
    m_ray_tracer->generate_ui_ray_tracing_logs();
    m_photon_mapper->generate_ui_logs();
    m_ray_tracer->generate_ui_photon_gathering_logs();
    ImGui::End();
}

void Photonear::dock_ui() {
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 viewport_pos = ImGui::GetMainViewport()->Pos;

    // ------- Docking Layout -------
    // |  1   |  2  |   7   |   8   |
    // |------|---------------------|
    // |  3   |     5    |    9     |
    // |------|---------------------|
    // |  4   |          6          |
    // ------- Docking Layout -------
    ImGuiID dock_id_1 = ImGui::DockBuilderAddNode(0, ImGuiDockNodeFlags_None);
    ImGui::DockBuilderSetNodePos(dock_id_1, viewport_pos);
    ImGui::DockBuilderSetNodeSize(dock_id_1, io.DisplaySize);
    ImGuiID dock_id_2 = ImGui::DockBuilderSplitNode(dock_id_1, ImGuiDir_Right, 0.8, nullptr,
                                                    &dock_id_1); // width 256789 in window
    ImGuiID dock_id_3 = ImGui::DockBuilderSplitNode(dock_id_1, ImGuiDir_Down, 0.65, nullptr,
                                                    &dock_id_1); // height 34 in 134
    ImGuiID dock_id_4 = ImGui::DockBuilderSplitNode(dock_id_3, ImGuiDir_Down, 0.54, nullptr,
                                                    &dock_id_3); // height 4 in 34
    ImGuiID dock_id_5 = ImGui::DockBuilderSplitNode(dock_id_2, ImGuiDir_Down, 0.75, nullptr,
                                                    &dock_id_2); // height 596 in 256789
    ImGuiID dock_id_6 = ImGui::DockBuilderSplitNode(dock_id_5, ImGuiDir_Down, 0.22, nullptr,
                                                    &dock_id_5); // height 6 in 596
    ImGuiID dock_id_7 = ImGui::DockBuilderSplitNode(dock_id_2, ImGuiDir_Right, 0.73, nullptr,
                                                    &dock_id_2); // width 78 in 278
    ImGuiID dock_id_8 = ImGui::DockBuilderSplitNode(dock_id_7, ImGuiDir_Right, 0.51, nullptr,
                                                    &dock_id_7); // width 8 in 78
    ImGuiID dock_id_9 = ImGui::DockBuilderSplitNode(dock_id_5, ImGuiDir_Right, 0.5, nullptr,
                                                    &dock_id_5); // width 9 in 59
    ImGui::DockBuilderDockWindow(SceneGraphEditorName, dock_id_1);
    ImGui::DockBuilderDockWindow(SceneSettingsName, dock_id_2);
    ImGui::DockBuilderDockWindow(NodeEditorName, dock_id_3);
    ImGui::DockBuilderDockWindow(ComponentEditorName, dock_id_4);
    ImGui::DockBuilderDockWindow(OpenGLViewerName, dock_id_5);
    ImGui::DockBuilderDockWindow(LogsName, dock_id_6);
    ImGui::DockBuilderDockWindow(RayTracingSettingsName, dock_id_7);
    ImGui::DockBuilderDockWindow(PhotonMappingSettingsName, dock_id_8);
    ImGui::DockBuilderDockWindow(PhotonMappingViewerName, dock_id_9);
    ImGui::DockBuilderFinish(dock_id_1);
}

void Photonear::finish() { // NOLINT(readability-convert-member-functions-to-static)
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Photonear::window_resize_callback(GLFWwindow *window, int width, int height) {
    Photonear::get_instance()->init_docking();
}

void Photonear::set_window(GLFWwindow *window) {
    m_window = window;
}

void Photonear::set_node_selected(AbstractNode *node_selected) {
    m_node_selected = node_selected;
    m_component_selected = nullptr;
}

AbstractNode *Photonear::get_node_selected() {
    return m_node_selected;
}

void Photonear::set_component_selected(Component *component_selected) {
    m_component_selected = component_selected;
}

Component *Photonear::get_component_selected() {
    return m_component_selected;
}

void Photonear::generate_ui_node_editor() {
    if (m_node_selected == nullptr) {
        std::string text = "No Node selected";
        auto window_size = ImGui::GetWindowSize();
        auto text_size = ImGui::CalcTextSize(text.c_str());
        ImGui::SetCursorPos(ImVec2((window_size.x - text_size.x) * 0.5f, (window_size.y - text_size.y) * 0.5f));
        ImGui::Text("%s", text.c_str());
    } else {
        m_node_selected->generate_ui_node_editor();
        auto components = Component::get_components(m_node_selected);
        for (const auto &component: components) {
            component->generate_ui_node_editor_ui();
        }
    }
}

void Photonear::generate_ui_component_editor() {
    if (m_component_selected == nullptr && m_node_selected != nullptr) {
        auto trsf_comp = &*Component::get_component<TransformComponent>(m_node_selected);
        if (trsf_comp->is_displayable()) {
            m_component_selected = trsf_comp;
        } else {
            auto components = Component::get_components(m_node_selected);
            if (!components.empty()) {
                for (const auto &component: components) {
                    if (component->is_displayable()) {
                        m_component_selected = &*component;
                        break;
                    }
                }
            }
        }
    }
    if (m_component_selected == nullptr) {
        std::string text = "No Component selected";
        auto window_size = ImGui::GetWindowSize();
        auto text_size = ImGui::CalcTextSize(text.c_str());
        ImGui::SetCursorPos(ImVec2((window_size.x - text_size.x) * 0.5f, (window_size.y - text_size.y) * 0.5f));
        ImGui::Text("%s", text.c_str());
    } else {
        m_component_selected->generate_ui_component_editor();
    }
}

std::shared_ptr<RayTracer> Photonear::get_ray_tracer() {
    return m_ray_tracer;
}

std::shared_ptr<PhotonMapper> Photonear::get_photon_mapper() {
    return m_photon_mapper;
}

std::shared_ptr<Scene> Photonear::get_scene() {
    return m_scene;
}

void Photonear::start_frame() { // NOLINT(readability-convert-member-functions-to-static)
    // Clear the UI screen
    glClearColor(0.1f, 0.1f, 0.1f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Init Dear Imgui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Photonear::finish_frame() { // NOLINT(readability-convert-member-functions-to-static)
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}