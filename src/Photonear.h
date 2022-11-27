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
    std::shared_ptr<Scene> m_scene;
    GLFWwindow* m_window;
    bool m_init_ui;
public:
    explicit Photonear(GLFWwindow* window);

    void reinit_ui();

    void draw(float delta_time);

    static void finish();
};

struct PhotonearResizeCallbackWrapper{
private:
    static PhotonearResizeCallbackWrapper Instance;
    Photonear* m_photonear;
    PhotonearResizeCallbackWrapper() = default;
    explicit PhotonearResizeCallbackWrapper(Photonear* photonear);
public:
    static void init(Photonear* photonear);

    static void window_resize_callback(GLFWwindow* window, int width,int height);
};

#endif //PHOTONEAR_PHOTONEAR_H
