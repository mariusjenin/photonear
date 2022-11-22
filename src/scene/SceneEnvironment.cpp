//
// Created by mariusjenin on 21/11/22.
//

#include "SceneEnvironment.h"
#include "memory"
#include "Node.h"
#include "Camera.h"

using namespace scene;
using namespace component;

SceneEnvironment::SceneEnvironment(GLFWwindow* window,
                                          const std::string &vertex_shader_path,
                                          const std::string &fragment_shader_path,
                                          std::shared_ptr<Scene> scene, vec3 clear_color,
                                          float fovy, float z_near, float z_far){
    m_window = window;
    m_shaders = std::make_shared<VertFragShaders>(vertex_shader_path.c_str(), fragment_shader_path.c_str());
    m_fovy = fovy;
    m_z_near = z_near;
    m_z_far = z_far;

    GLuint program_id = m_shaders->get_program_id();
    m_shaders->use();

    m_scene = scene;

    ShadowMapShaders* shadow_map_shaders = m_shaders->get_shadow_map_shaders();

    //LOCATION DATA
    m_shaders->load_location();
    shadow_map_shaders->load_location();

    //UNIFORM DEFINED VALUES
    ShadersDataManager *shader_data_manager = m_shaders->get_shader_data_manager();
    shader_data_manager->load_material_const(program_id);
    shader_data_manager->load_lights_const(program_id);
    shader_data_manager->load_debug_const(program_id);

    glClearColor(clear_color.x,clear_color.y,clear_color.z,0);
}

void SceneEnvironment::render() {
    std::shared_ptr<AbstractNode> curr_node = m_scene->get_root_node();
    draw_node_recursive(Material::get_default(), curr_node);

    load_projection_matrix();

    //CAMERA
    auto cams = Component::get_components<Camera>();
    std::shared_ptr<Camera> camera = nullptr;
    for(const auto& cam : cams){
        if(camera == nullptr || cam->get_priority() < camera->get_priority()){
            camera = cam;
        }
    }

    auto node_camera = Component::get_node(camera);
    Transform trsf_tmp = Transform(node_camera->get_matrix_recursive_local(false));
    Camera::update_view_mat(trsf_tmp,m_shaders);
    Camera::update_view_pos(trsf_tmp,m_shaders);

//    load_lights(); //TODO
}


void SceneEnvironment::load_projection_matrix() {
    int width,height;
    glfwGetWindowSize(m_window, &width, &height);

    //PROJECTION
    mat4 projection_mat = perspective(radians(m_fovy), (float)width/(float)height, m_z_near, m_z_far);
    glUniformMatrix4fv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::PROJ_MAT_LOC_NAME), 1,
                       GL_FALSE, &projection_mat[0][0]);
}

void SceneEnvironment::draw_node_recursive(const std::shared_ptr<Material>& parent_material, const std::shared_ptr<AbstractNode>& parent_node) {
    auto children_node = parent_node->get_children();
    for(auto node : children_node){
        auto node_mat = Component::get_component<Material>(node);
        auto material = node_mat != nullptr ? node_mat : parent_material;
        auto hittables = Component::get_components<Hittable>(node);

        for(auto hittable : hittables){
            draw_shape(material, hittable);
        }

        draw_node_recursive(material, node);
    }
}

void SceneEnvironment::draw_shape(std::shared_ptr<Material> material, const std::shared_ptr<Hittable>& hittable) {
    ShadersDataManager *shader_data_manager= shader_data_manager = m_shaders->get_shader_data_manager();

//    load_model_matrices(m_shaders); TODO Node -> SceneEnvironment

//    if (m_see_both_face) {
//        glDisable(GL_CULL_FACE);
//    } else {
//        glEnable(GL_CULL_FACE);
//    }


//    material->load_in_shader(m_shaders); TODO MATERIAL

    ShadersBufferManager::bind_vao(hittable->get_vao_id());
    ShadersBufferManager::draw(hittable->get_ebo_triangle_indices_id(), (long) hittable->get_triangle_indices().size());
}

