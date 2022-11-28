//
// Created by mariusjenin on 27/11/22.
//

#include "Photonear.h"
#include "CornellBox.h"

Photonear *Photonear::PhotonearInstance = nullptr;

const char * Photonear::SceneGraphEditorName = "Scene Graph Editor";
const char * Photonear::NodeEditorName = "Node Editor";
const char * Photonear::ComponentEditorName = "Component Editor";
const char * Photonear::OpenGLViewerName = "OpenGL Viewer";
const char * Photonear::PhotonMappingViewerName = "Photon Mapping Viewer";
const char * Photonear::SceneSettingsName = "Scene Settings";
const char * Photonear::PhotonMappingSettingsName = "Photon Mapping Settings";
const char * Photonear::LogsName = "Logs";

Photonear *Photonear::get_instance() {
    if (PhotonearInstance == nullptr) {
        PhotonearInstance = new Photonear();
    }
    return PhotonearInstance;
}

void Photonear::init() {
    m_init_ui = true;
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

    m_scene->init();

//  TODO BB and raycast  std::cout << abi::__cxa_demangle(typeid(*Component::get_component<Camera>()).name(), 0, 0, nullptr) << std::endl;
}

void Photonear::reinit_ui() {
    m_init_ui = true;
}

void Photonear::draw(float delta_time) {

    // Clear the UI screen
    glClearColor(0.1f, 0.1f, 0.1f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Init Dear Imgui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    if (m_init_ui) {
        dock_ui();
        m_init_ui = false;
    }

    auto window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin(SceneGraphEditorName, nullptr, window_flags);
    m_scene->get_scene_graph()->generate_scene_graph_ui();
    ImGui::End();
    ImGui::Begin(NodeEditorName, nullptr, window_flags);
    generate_ui_node_editor();
    ImGui::End();
    ImGui::Begin(ComponentEditorName, nullptr, window_flags);
    generate_ui_component_editor();
    ImGui::End();

    ImGui::Begin(OpenGLViewerName, nullptr, window_flags);
    auto window_size = ImGui::GetWindowSize();
    auto cursor_pos = ImGui::GetCursorScreenPos();

    //RENDER SCENE
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    m_scene->set_viewer_size((int) window_size.x, (int) window_size.y);
    m_scene->update(delta_time);
    m_scene->draw();

    ImGui::GetWindowDrawList()->AddImage(
            reinterpret_cast<ImTextureID>(m_scene->get_texture()),
            cursor_pos,
            ImVec2(cursor_pos.x + window_size.x - 15, cursor_pos.y + window_size.y - 35),
            ImVec2(0, 1), ImVec2(1, 0)
    );
    ImGui::End();
    ImGui::Begin(PhotonMappingViewerName, nullptr, window_flags);
    ImGui::End();
    ImGui::Begin(SceneSettingsName, nullptr, window_flags);
    ImGui::End();
    ImGui::Begin(PhotonMappingSettingsName, nullptr, window_flags);
    ImGui::End();
    ImGui::Begin(LogsName, nullptr, window_flags);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Photonear::dock_ui(){
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 viewport_pos = ImGui::GetMainViewport()->Pos;

    // ------- Docking Layout -------
    // |  1   |     2    |    7     |
    // |------|---------------------|
    // |  3   |     5    |    8     |
    // |------|---------------------|
    // |  4   |          6          |
    // ------- Docking Layout -------
    ImGuiID dock_id_1 = ImGui::DockBuilderAddNode(0, ImGuiDockNodeFlags_None);
    ImGui::DockBuilderSetNodePos(dock_id_1, viewport_pos);
    ImGui::DockBuilderSetNodeSize(dock_id_1, io.DisplaySize);
    ImGuiID dock_id_2 = ImGui::DockBuilderSplitNode(dock_id_1, ImGuiDir_Right, 0.8, nullptr,
                                                    &dock_id_1); // width 25678 in window
    ImGuiID dock_id_3 = ImGui::DockBuilderSplitNode(dock_id_1, ImGuiDir_Down, 0.65, nullptr,
                                                    &dock_id_1); // height 34 in 134
    ImGuiID dock_id_4 = ImGui::DockBuilderSplitNode(dock_id_3, ImGuiDir_Down, 0.54, nullptr,
                                                    &dock_id_3); // height 4 in 34
    ImGuiID dock_id_5 = ImGui::DockBuilderSplitNode(dock_id_2, ImGuiDir_Down, 0.75, nullptr,
                                                    &dock_id_2); // height 586 in 25678
    ImGuiID dock_id_6 = ImGui::DockBuilderSplitNode(dock_id_5, ImGuiDir_Down, 0.33, nullptr,
                                                    &dock_id_5); // height 6 in 586
    ImGuiID dock_id_7 = ImGui::DockBuilderSplitNode(dock_id_2, ImGuiDir_Right, 0.5, nullptr,
                                                    &dock_id_2); // width 7 in 27
    ImGuiID dock_id_8 = ImGui::DockBuilderSplitNode(dock_id_5, ImGuiDir_Right, 0.5, nullptr,
                                                    &dock_id_5); // width 8 in 58
    ImGui::DockBuilderDockWindow(SceneGraphEditorName, dock_id_1);
    ImGui::DockBuilderDockWindow(SceneSettingsName, dock_id_2);
    ImGui::DockBuilderDockWindow(NodeEditorName, dock_id_3);
    ImGui::DockBuilderDockWindow(ComponentEditorName, dock_id_4);
    ImGui::DockBuilderDockWindow(OpenGLViewerName, dock_id_5);
    ImGui::DockBuilderDockWindow(LogsName, dock_id_6);
    ImGui::DockBuilderDockWindow(PhotonMappingSettingsName, dock_id_7);
    ImGui::DockBuilderDockWindow(PhotonMappingViewerName, dock_id_8);
    ImGui::DockBuilderFinish(dock_id_1);
}

void Photonear::finish() { // NOLINT(readability-convert-member-functions-to-static)
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Photonear::window_resize_callback(GLFWwindow *window, int width, int height) {
    Photonear::get_instance()->reinit_ui();
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
    if(m_node_selected != nullptr){
        auto components = Component::get_components(m_node_selected);
        for(const auto& component:components){
            component->generate_node_editor_ui();
        }
    }
}


void Photonear::generate_ui_component_editor() {
    if(m_component_selected != nullptr){
        m_component_selected->generate_component_editor_ui();
    }
}
