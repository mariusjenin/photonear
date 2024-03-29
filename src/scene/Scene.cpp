//
// Created by mariusjenin on 21/11/22.
//

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "EmissiveMaterial.h"
#include "TextureManager.h"
#include "Photonear.h"
#include "BoundingBox.h"
#include "ShadersBufferManager.h"

using namespace std;
using namespace scene;
using namespace component::shape;
using namespace component::material;

Scene::Scene(GLFWwindow *window,
             const std::string &vertex_shader_path,
             const std::string &fragment_shader_path) {
    m_window = window;
    m_scene_valid = false;
    m_viewer_valid = false;
    m_auto_draw = true;
    m_camera_valid = false;
    m_debug_enabled = true;
    m_debug_depth = 2;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    m_default_color = {0, 0, 0};
    m_debug_color = {0, 1, 0.5f};
    m_debug_color_2 = {1, 0, 0.5f};

    m_shaders = std::make_shared<VertFragShaders>(vertex_shader_path.c_str(), fragment_shader_path.c_str());

    GLuint program_id = m_shaders->get_program_id();
    m_shaders->use();

    generate_texture();

    auto shadow_map_shaders = m_shaders->get_shadow_map_shaders();

    //LOCATION DATA
    m_shaders->load_location();
    shadow_map_shaders->load_location();

    //UNIFORM DEFINED VALUES
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    shader_data_manager->load_material_const(program_id);
    shader_data_manager->load_lights_const(program_id);
    shader_data_manager->load_debug_const(program_id);
}

void Scene::init() {
    init_scene_graph();
    m_scene_graph->compute_scene_graph();
    m_scene_graph->generate_bounding_boxes();
    init_camera();
}

void Scene::update(float delta_time) {
    handle_inputs(delta_time);
}

void Scene::load_camera() {
    std::shared_ptr<Camera> camera = get_active_camera();

    m_camera_valid = camera != nullptr;
    if (m_camera_valid) {
        camera->load_in_shaders(m_shaders, m_width_viewer, m_height_viewer);
    }
}

void Scene::draw(bool force) {
    if ((m_auto_draw && (!m_scene_valid || !m_viewer_valid)) || force) {
        load_camera();
        if (!m_camera_valid) return;
        load_lights();

        m_scene_graph->compute_bounding_boxes();

        glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

        glClearColor(m_default_color.x, m_default_color.y, m_default_color.z, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        draw_shapes(m_shaders);

        if (m_debug_enabled) draw_debug();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (!m_scene_valid) {
            Photonear::get_instance()->get_ray_tracer()->set_ray_tracing_valid(false);
            Photonear::get_instance()->get_photon_mapper()->set_photon_mapping_valid(false);
        }
        m_scene_valid = true;
        m_viewer_valid = true;
    }
}

void Scene::draw_debug() {

    auto shader_data_manager = m_shaders->get_shader_data_manager();
    glUniform1i(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_LOC_NAME), true);
    glUniform3fv(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_COLOR_LOC_NAME), 1,
                 &m_debug_color[0]);

    auto component_selected = Photonear::get_instance()->get_component_selected();
    if (component_selected != nullptr) {
        component_selected->draw();
    }
    auto node = Photonear::get_instance()->get_node_selected();
    if (node != nullptr) {
        auto bounding_box_node_selected = Component::get_component<BoundingBox>(node);
        if (bounding_box_node_selected != nullptr) {
            bounding_box_node_selected->draw();
        }
        glUniform3fv(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_COLOR_LOC_NAME), 1,
                     &m_debug_color_2[0]);
        auto bounding_box_children = Component::get_children_components_recursive<BoundingBox>(&*node, m_debug_depth);
        for (const auto &bb: bounding_box_children) {
            bb->draw();
        }
    }

    glUniform1i(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_LOC_NAME), false);
}


void Scene::draw_shapes(const std::shared_ptr<Shaders> &shaders) {
    auto shapes = Component::get_components<Shape>();
    for (const auto &shape: shapes) {
        auto node = Component::get_node(&*shape);
        if (node->is_active_recursive()) {
            shape->draw(shaders);
        }
    }
}

void Scene::set_viewer_size(int width, int height) {
    if (height != m_height_viewer || width != m_width_viewer) {
        m_viewer_valid = false;
        m_width_viewer = width;
        m_height_viewer = height;
        resize_texture();
    }
}

std::vector<EmissiveMaterial *> Scene::get_lights(){
    auto materials = Component::get_components<Material>();
    std::vector<EmissiveMaterial *> light_materials = {};
    for (const auto &material: materials) {
        if (material->is_emissive()) {
            auto node = Component::get_node(&*material);
            if (node->is_active_recursive()) {
                light_materials.push_back((EmissiveMaterial *) &*material);
            }
        }
    }
    return light_materials;
}

void Scene::load_lights() {
    // Retrieve all the lights
    auto shadow_map_shaders = m_shaders->get_shadow_map_shaders();
    auto light_materials = get_lights();
    m_nb_lights = (int)light_materials.size();

    // Init Data structure
    LightShader lights_shader[m_nb_lights];
    GLint depth_maps[NB_MAX_LIGHTS];
    for (int &depth_map: depth_maps) { depth_map = 0; }

    int count_index_depth_map = 0;

    std::vector<std::shared_ptr<ShadowMap>> shadow_maps = {};

    // Use the depth shader
    shadow_map_shaders->use();
    for (int i = 0; i < m_nb_lights; i++) {
        Light light = light_materials[i]->generate_light();
        lights_shader[i] = LightShader(light);
        // Generate the Shadow if it has to
        if (light.get_generate_depth_map()) {
            auto shadow_map = light.get_shadow_map();
            shadow_maps.push_back(shadow_map);
            // Bind the buffer, draw the depth map and unbind it
            shadow_map->bind();
            // Load de depth light VP into the depth pass
            light.load_depth_vp_matrix(&*shadow_map_shaders);
            // Render the depth map
            draw_shapes(shadow_map_shaders);
            // Print the depth map
            // shadow_map->print_in_img((std::string("../depth_map_light_")+std::to_string(count_index_depth_map)+std::string(".ppm")).c_str());
            // Unbind the ShadowMap
            ShadowMap::unbind_bound_shadow_map();
            // Get the right shadow map index
            int index_shadow_map = (int)light.get_index_sampler_depth_map();
            depth_maps[count_index_depth_map] = index_shadow_map;
            lights_shader[i].index_shadow_map = count_index_depth_map;
            count_index_depth_map++;
        }
    }
    // Put shadow maps in texture in the main shader
    m_shaders->use();

    for (auto &shadow_map: shadow_maps) {
        shadow_map->activate_texture();
    }
    glUniform1iv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::SHADOW_MAP_ARRAY_LOC_NAME),
                 NB_MAX_LIGHTS, depth_maps);
    m_shaders->get_shader_data_manager()->load_lights(m_shaders->get_program_id(), lights_shader, (int) m_nb_lights);

    glViewport(0, 0, m_width_viewer, m_height_viewer);
}

void Scene::handle_inputs(float delta_time) {
    float camera_speed = 7.f * delta_time;
    float camera_speed_rot = 100 * delta_time;

    std::shared_ptr<Camera> camera = get_active_camera();
    if (camera == nullptr) return;
    auto camera_node = Component::get_node(&*camera);

    auto camera_trsf = Component::get_component<TransformComponent>(&*camera_node)->get_transform();

    //Camera Translation
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        vec3 dir = {};
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            dir += vec3(camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            dir += vec3(-camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
            dir += vec3(0.f, camera_speed, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
            dir += vec3(0.f, -camera_speed, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            dir += vec3(0.f, 0.f, +camera_speed);
        }
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            dir += vec3(0.f, 0.f, -camera_speed);
        }
        camera_trsf->set_translation(camera_trsf->get_translation() + camera_trsf->apply_to_vector(dir));
    } else {
        vec3 rot = {};
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            rot += vec3(camera_speed_rot, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            rot -= vec3(camera_speed_rot, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            rot += vec3(0.f, camera_speed_rot, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            rot -= vec3(0.f, camera_speed_rot, 0.f);
        }
        camera_trsf->set_rotation(camera_trsf->get_rotation() + rot);
    }
    if (!camera_trsf->is_up_to_date()) {
        camera_trsf->compute();
        m_scene_valid = false;
    }
}

void Scene::generate_texture() {
    // Create and bind FrameBuffer
    glGenFramebuffers(1, &m_frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Create and Bind Texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width_viewer, m_height_viewer, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    // Create and bind RenderBuffer
    glGenRenderbuffers(1, &m_render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width_viewer, m_height_viewer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::resize_texture() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width_viewer, m_height_viewer, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width_viewer, m_height_viewer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<SceneGraph> Scene::get_scene_graph() {
    return m_scene_graph;
}

void Scene::set_scene_valid(bool valid) {
    m_scene_valid = valid;
}

void Scene::set_viewer_valid(bool valid) {
    m_viewer_valid = valid;
}

void Scene::generate_ui_scene_settings() {
    ImGui::Checkbox("Auto draw Scene", &m_auto_draw);
    if (ImGui::Button("Draw")) {
        if (m_scene_valid) {
            Photonear::get_instance()->get_ray_tracer()->set_ray_tracing_valid(false);
        }
        load_camera();
        load_lights();
        draw(true);
    }
    ImGui::Separator();
    color default_color = m_default_color;
    ImGui::ColorEdit3("Default Color", &default_color[0]);

    ImGui::Separator();
    color debug_color = m_debug_color;
    color debug_color_2 = m_debug_color_2;
    bool debug_enabled = m_debug_enabled;
    int debug_depth = m_debug_depth;
    ImGui::Checkbox("Debugging", &debug_enabled);
    ImGui::ColorEdit3("Debugging Color", &debug_color[0]);
    ImGui::ColorEdit3("Debugging Color 2", &debug_color_2[0]);
    ImGui::DragInt("Debug Depth", &debug_depth, 0.01f, 0, INT_MAX);
    if (m_debug_color != debug_color ||
        m_debug_enabled != debug_enabled ||
        m_debug_color_2 != debug_color_2 ||
        m_debug_depth != debug_depth ||
        m_default_color != default_color) {
        m_viewer_valid = false;
    }
    m_default_color = default_color;
    m_debug_color = debug_color;
    m_debug_color_2 = debug_color_2;
    m_debug_enabled = debug_enabled;
    m_debug_depth = debug_depth;

}

void Scene::generate_ui_viewer() const {
    if (m_camera_valid) {
        auto cursor_pos = ImGui::GetCursorScreenPos();

        ImGui::GetWindowDrawList()->AddImage(
                reinterpret_cast<ImTextureID>(m_texture),
                cursor_pos,
                ImVec2(cursor_pos.x + (float) m_width_viewer - 15, cursor_pos.y + (float) m_height_viewer - 35),
                ImVec2(0, 1), ImVec2(1, 0)
        );
    } else {
        std::string text = "No Camera Active";
        auto window_size = ImGui::GetWindowSize();
        auto text_size = ImGui::CalcTextSize(text.c_str());
        ImGui::SetCursorPos(ImVec2((window_size.x - text_size.x) * 0.5f, (window_size.y - text_size.y) * 0.5f));
        ImGui::Text("%s", text.c_str());
    }
}

void Scene::init_camera() {
    auto camera = get_active_camera();
    if (camera != nullptr) {
        camera->set_capturing(true);
        m_camera_valid = true;
    }
}

std::shared_ptr<Camera> Scene::get_active_camera() {
    auto cameras = Component::get_components<Camera>();
    std::shared_ptr<Camera> camera;
    for (const auto &cam: cameras) {
        if (camera == nullptr || cam->is_capturing()) {
            auto node = Component::get_node(&*cam);
            if (node->is_active_recursive()) {
                camera = cam;
            }
        }
    }
    return camera;
}

int Scene::get_nb_lights() const {
    return m_nb_lights;
}
