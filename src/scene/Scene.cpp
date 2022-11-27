//
// Created by mariusjenin on 21/11/22.
//

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "LightMaterial.h"
#include "TextureManager.h"
#include "NodeFactory.h"

using namespace std;
using namespace scene;
using namespace component::shape;
using namespace component::material;

Scene::Scene(GLFWwindow* window,
             const std::string &vertex_shader_path,
             const std::string &fragment_shader_path, vec3 clear_color){
    m_window = window;

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    m_clear_color = clear_color;

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
}

void Scene::update(float delta_time){
    glClearColor(m_clear_color.x,m_clear_color.y,m_clear_color.z,0);
    handle_inputs(delta_time);

    //CAMERA
    auto cams = Component::get_components<Camera>();
    std::shared_ptr<Camera> camera = nullptr;
    for(const auto& cam : cams){
        if(camera == nullptr || cam->get_priority() < camera->get_priority()){
            camera = cam;
        }
    }
    if(camera == nullptr){
        std::cerr << "No camera found in the scene" << std::endl;
        exit(1);
    }
    camera->load_in_shaders(m_shaders,m_width_viewer,m_height_viewer);

    load_lights();

    glViewport(0, 0,m_width_viewer, m_height_viewer);
}

void Scene::draw() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_shapes(m_shaders);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::draw_shapes(const std::shared_ptr<Shaders>& shaders){
    auto shapes = Component::get_components<Shape>();
    for(const auto& shape : shapes){
        shape->draw(shaders);
    }
}

void Scene::set_viewer_size(int width, int height){
    if(height != m_height_viewer || width != m_width_viewer){
        m_width_viewer = width;
        m_height_viewer = height;
        resize_texture();
    }
}

void Scene::load_lights(){
    // Retrieve all the lights
    auto shadow_map_shaders = m_shaders->get_shadow_map_shaders();
    auto materials = Component::get_components<Material>();
    std::vector<LightMaterial*> light_materials = {};
    for(const auto& material: materials){
        if(material->is_emissive()){
            light_materials.push_back((LightMaterial*)&*material);
        }
    }
    size_t size_lights = light_materials.size();

    // Init Data structure
    LightShader lights_shader[size_lights];
    GLint depth_maps[NB_MAX_LIGHTS];
    for(int & depth_map : depth_maps){depth_map = 0;}

    int count_index_depth_map = 0;

    std::vector<std::shared_ptr<ShadowMap>> shadow_maps = {};

    // Use the depth shader
    shadow_map_shaders->use();
    for(int i = 0 ; i < size_lights; i++){
        Light light = light_materials[i]->generate_light();
        lights_shader[i] = LightShader(light);
        // Generate the Shadow if it has to
        if(light.get_generate_depth_map()){
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
            int index_shadow_map = light.get_index_sampler_depth_map();
            depth_maps[count_index_depth_map] = index_shadow_map;
            lights_shader[i].index_shadow_map = count_index_depth_map;
            count_index_depth_map++;
        }
    }
    // Put shadow maps in texture in the main shader
    m_shaders->use();

    for(auto & shadow_map : shadow_maps){
        shadow_map->activate_texture();
    }
    glUniform1iv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::SHADOW_MAP_ARRAY_LOC_NAME),NB_MAX_LIGHTS, depth_maps);
    m_shaders->get_shader_data_manager()->load_lights(m_shaders->get_program_id(), lights_shader,(int)size_lights);
}

void Scene::handle_inputs(float delta_time) {
    float camera_speed = 7.f * delta_time;
    float camera_speed_rot = 100 * delta_time;

    auto camera_node = Component::get_node_having<Camera>();
    auto camera_trsf = Component::get_component<TransformComponent>(&*camera_node)->get_transform();

    //Camera Translation
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        glm::vec3 dir = {};
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            dir += glm::vec3(camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            dir += glm::vec3(-camera_speed, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
            dir += glm::vec3(0.f, camera_speed, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
            dir += glm::vec3(0.f, -camera_speed, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            dir += glm::vec3(0.f, 0.f, +camera_speed);
        }
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            dir += glm::vec3(0.f, 0.f, -camera_speed);
        }
        camera_trsf->set_translation(camera_trsf->get_translation() + camera_trsf->apply_to_vector(dir));
    } else {
        glm::vec3 rot = {};
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            rot += glm::vec3(camera_speed_rot, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            rot -= glm::vec3(camera_speed_rot, 0.f, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            rot += glm::vec3(0.f, camera_speed_rot, 0.f);
        }
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            rot -= glm::vec3(0.f, camera_speed_rot, 0.f);
        }
        camera_trsf->set_rotation(camera_trsf->get_rotation() + rot);
    }
    if (!camera_trsf->is_up_to_date()) camera_trsf->compute();
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
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer );
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


GLuint Scene::get_texture() const {
    return m_texture;
}
