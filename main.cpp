#include <iostream>
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Node.h"
#include "NodeFactory.h"
#include "ComponentFactory.h"
#include "Shape.h"
#include "Scene.h"
#include "Component.h"

using namespace scene;
using namespace scene::node;

GLFWwindow *window;

void window_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //Get the primary monitor size and pos
    int pos_monitor_x, pos_monitor_y;
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorPos(monitor, &pos_monitor_x, &pos_monitor_y);

    // Open a window and create its OpenGL context
    const unsigned int WINDOW_WIDTH = 800;
    const unsigned int WINDOW_HEIGHT = 600;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Photonear", nullptr, nullptr);

    //Get the primary monitor size and pos
    glfwSetWindowPos(window, pos_monitor_x, pos_monitor_y);

    if (window == nullptr) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    //Callback resize
    glfwSetWindowSizeCallback(window, window_size_callback);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, WINDOW_WIDTH / 2., WINDOW_HEIGHT / 2.);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // For speed computation
    auto last_time = (float) glfwGetTime();
    float t;

    //Frame updates
    int fps = 60;
    float dt_frame_acc = 0.0f;
    float dt_frame_fixed = 1.0f / (float) fps;

    //CREATE THE SCENE
    auto root = NodeFactory::create_root_node();
    auto camera_node = NodeFactory::create_node(root);
    auto triangle_node = NodeFactory::create_node(root);
    auto sphere_node = NodeFactory::create_node(triangle_node);

    auto camera = ComponentFactory::create_camera();
    auto sphere = ComponentFactory::create_sphere(true);
    Component::add_component_to_node<Shape>(sphere, triangle_node);
    Component::add_component_to_node<Camera>(camera, camera_node);


    auto trsf_camera = Component::get_component<TransformComponent>(&*camera_node)->get_transform();
    trsf_camera->set_translation({0,0,5});

    auto trsf_shape = Component::get_component<TransformComponent>(&*triangle_node)->get_transform();
    trsf_shape->set_translation({0, 0, 0});


    auto scene_graph = std::make_shared<SceneGraph>(root);
    auto scene = Scene(
            window,
            "../src/shader/vertex_shader.glsl",
            "../src/shader/fragment_shader.glsl",
            scene_graph,
            {0.1f,0.1f,0.1f});

    do {
        t = (float) glfwGetTime();
        dt_frame_acc += t - last_time;

        if (dt_frame_acc > dt_frame_fixed) {
            dt_frame_acc -= dt_frame_fixed;
            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //RENDER SCENE
            scene.draw();

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        last_time = t;

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}