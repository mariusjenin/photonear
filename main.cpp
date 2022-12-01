//
// Created by mariusjenin on 19/11/22.
//

#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Photonear.h"

GLFWwindow *window_photonear;

int main() {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    // GLFW Parameter
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Get the primary monitor size, pos and video mdoe
    int pos_monitor_x, pos_monitor_y;
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorPos(monitor, &pos_monitor_x, &pos_monitor_y);
    auto video_mode = glfwGetVideoMode(monitor);
    int fps = video_mode->refreshRate;

    // Open a window and create its OpenGL context
    window_photonear = glfwCreateWindow(video_mode->width, video_mode->height, "Photonear", nullptr, nullptr);

    //Get the primary monitor size and pos
    glfwSetWindowPos(window_photonear, pos_monitor_x, pos_monitor_y);

    if (window_photonear == nullptr) {
        fprintf(stderr, "Failed to open GLFW window. \n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window_photonear);

    // Enable vsync
    glfwSwapInterval(1);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window_photonear, GLFW_STICKY_KEYS, GL_TRUE);

    Photonear* photonear = Photonear::get_instance();
    photonear->set_window(window_photonear);

    photonear->init();

    // For speed computation
    auto last_time = (float) glfwGetTime();
    float t;
    float dt_frame_acc = 0.0f;
    float dt_frame_fixed = 1.0f / (float) fps;

    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window_photonear, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window_photonear) == 0) {
        glfwPollEvents();
        t = (float) glfwGetTime();
        dt_frame_acc += t - last_time;

        if (dt_frame_acc > dt_frame_fixed) {
            dt_frame_acc -= dt_frame_fixed;

            photonear->start_frame();
            photonear->update(dt_frame_fixed);
            photonear->draw();
            photonear->finish_frame();

            // Swap buffers
            glfwSwapBuffers(window_photonear);
        }

        last_time = t;
    }

    photonear->finish();

    // Close OpenGL window and terminate GLFW
    glfwDestroyWindow(window_photonear);
    glfwTerminate();

    return 0;
}