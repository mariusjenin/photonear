//
// Created by mariusjenin on 27/11/22.
//

#include "Photonear.h"
#include "CornellBox.h"

Photonear::Photonear(GLFWwindow* window) {
    m_init_ui = true;
    m_window = window;
    m_scene = std::make_shared<CornellBox>(window, "../src/shader/vertex_shader.glsl", "../src/shader/fragment_shader.glsl");

    //Callback resize
    PhotonearResizeCallbackWrapper::init(this);
    glfwSetWindowSizeCallback(m_window, PhotonearResizeCallbackWrapper::window_resize_callback);

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
    ImGuiIO &io = ImGui::GetIO();

    // Clear the UI screen
    glClearColor(0.1f, 0.1f, 0.1f, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Init Dear Imgui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    auto scene_graph_editor_name = "Scene Graph Editor";
    auto node_editor_name = "Node Editor";
    auto component_editor_name = "Component Editor";
    auto opengl_viewer_name = "OpenGL Viewer";
    auto photon_mapping_viewer_name = "Photon Mapping Viewer";
    auto scene_settings_name = "Scene Settings";
    auto photon_mapping_settings_name = "Photon Mapping Settings";
    auto logs_name = "Logs";
    ImVec2 viewport_pos = ImGui::GetMainViewport()->Pos;

    if(m_init_ui){
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
        ImGuiID dock_id_2 = ImGui::DockBuilderSplitNode(dock_id_1, ImGuiDir_Right, 0.8, nullptr, &dock_id_1);
        ImGuiID dock_id_3 = ImGui::DockBuilderSplitNode(dock_id_1, ImGuiDir_Down, 0.6, nullptr, &dock_id_1);
        ImGuiID dock_id_4 = ImGui::DockBuilderSplitNode(dock_id_3, ImGuiDir_Down, 0.6, nullptr, &dock_id_3);
        ImGuiID dock_id_5 = ImGui::DockBuilderSplitNode(dock_id_2, ImGuiDir_Down, 0.75, nullptr, &dock_id_2);
        ImGuiID dock_id_6 = ImGui::DockBuilderSplitNode(dock_id_5, ImGuiDir_Down, 0.35, nullptr, &dock_id_5);
        ImGuiID dock_id_7 = ImGui::DockBuilderSplitNode(dock_id_2, ImGuiDir_Right, 0.5, nullptr, &dock_id_2);
        ImGuiID dock_id_8 = ImGui::DockBuilderSplitNode(dock_id_5, ImGuiDir_Right, 0.5, nullptr, &dock_id_5);
        ImGui::DockBuilderDockWindow(scene_graph_editor_name, dock_id_1);
        ImGui::DockBuilderDockWindow(scene_settings_name, dock_id_2);
        ImGui::DockBuilderDockWindow(component_editor_name, dock_id_3);
        ImGui::DockBuilderDockWindow(node_editor_name, dock_id_4);
        ImGui::DockBuilderDockWindow(opengl_viewer_name, dock_id_5);
        ImGui::DockBuilderDockWindow(logs_name, dock_id_6);
        ImGui::DockBuilderDockWindow(photon_mapping_settings_name, dock_id_7);
        ImGui::DockBuilderDockWindow(photon_mapping_viewer_name, dock_id_8);
        ImGui::DockBuilderFinish(dock_id_1);
    }

    auto window_flags = ImGuiWindowFlags_NoMove;

    ImGui::Begin(scene_graph_editor_name, nullptr, window_flags);
    ImGui::End();
    ImGui::Begin(node_editor_name, nullptr, window_flags);
    ImGui::End();
    ImGui::Begin(component_editor_name, nullptr, window_flags);
    ImGui::End();

    ImGui::Begin(opengl_viewer_name, nullptr, window_flags);
    auto window_size = ImGui::GetWindowSize();
    auto cursor_pos = ImGui::GetCursorScreenPos();

    //RENDER SCENE
    int width, height;
    glfwGetWindowSize(m_window,&width,&height);
    m_scene->set_viewer_size((int)window_size.x, (int)window_size.y);
    m_scene->update(delta_time);
    m_scene->draw();

    ImGui::GetWindowDrawList()->AddImage(
            reinterpret_cast<ImTextureID>(m_scene->get_texture()),
            cursor_pos,
            ImVec2(cursor_pos.x + window_size.x - 15, cursor_pos.y + window_size.y - 35),
            ImVec2(0, 1),ImVec2(1, 0)
    );
    ImGui::End();
    ImGui::Begin(photon_mapping_viewer_name, nullptr, window_flags);
    ImGui::End();
    ImGui::Begin(scene_settings_name, nullptr, window_flags);
    ImGui::End();
    ImGui::Begin(photon_mapping_settings_name, nullptr, window_flags);
    ImGui::End();
    ImGui::Begin(logs_name, nullptr, window_flags);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_init_ui = false;
}

void Photonear::finish() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

PhotonearResizeCallbackWrapper PhotonearResizeCallbackWrapper::Instance = PhotonearResizeCallbackWrapper();

void PhotonearResizeCallbackWrapper::window_resize_callback(GLFWwindow* window, int width,int height) {
//    static PhotonearResizeCallbackWrapper Instance;
    Instance.m_photonear->reinit_ui();
}

PhotonearResizeCallbackWrapper::PhotonearResizeCallbackWrapper(Photonear *photonear) {
    m_photonear=photonear;
}

void PhotonearResizeCallbackWrapper::init(Photonear *photonear) {
    Instance = PhotonearResizeCallbackWrapper(photonear);
}
