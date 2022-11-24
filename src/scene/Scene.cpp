//
// Created by mariusjenin on 21/11/22.
//

#include "Scene.h"

#include <utility>
#include "memory"
#include "Node.h"
#include "Camera.h"
#include "ShadersBufferManager.h"

using namespace scene;
using namespace component;

Scene::Scene(GLFWwindow* window,
             const std::string &vertex_shader_path,
             const std::string &fragment_shader_path,
             std::shared_ptr<SceneGraph> scene, vec3 clear_color){
    m_window = window;
    m_shaders = std::make_shared<VertFragShaders>(vertex_shader_path.c_str(), fragment_shader_path.c_str());

    GLuint program_id = m_shaders->get_program_id();
    m_shaders->use();

    m_scene_graph = std::move(scene);
    m_scene_graph->compute_scene_graph();

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

void Scene::draw() {
    //CAMERA
    auto cams = Component::get_components<Camera>();
    std::shared_ptr<Camera> camera = nullptr;
    for(const auto& cam : cams){
        if(camera == nullptr || cam->get_priority() < camera->get_priority()){
            camera = cam;
        }
    }
    if(camera == nullptr){
        std::cerr << "Pas de caméra trouvée dans la scène" << std::endl;
        exit(1);
    }
    camera->load_in_shaders(m_window, m_shaders);

    // SHAPES
    auto shapes = Component::get_components<Shape>();
    for(const auto& shape : shapes){
        shape->draw(m_shaders);
    }

//    load_lights(); //TODO
}


//void Scene::load_lights(){
//    ShadowMapShaders* shadow_map_shaders = m_shaders->get_shadow_map_shaders();
//
//    size_t size_lights = m_lights.size();
//
//    LightShader lights_struct_array[size_lights];
//
//    GLint depth_maps[NB_MAX_LIGHTS];
//    for(int & depth_map : depth_maps){depth_map = 0;}
//
//    int count_index_depth_map = 0;
//
//    std::vector<ShadowMap*> shadow_maps = {};
//
//    //Use the depth shader
//    shadow_map_shaders->use();
//    for(int i = 0 ; i < size_lights; i++){
//        LightInfo light_info = m_lights[i]->generate_light_struct();
//        lights_struct_array[i] = LightShader(light_info);
//        //Generate the Shadow if it has to
//        if(light_info.generate_depth_map){
//            shadow_maps.push_back(light_info.shadow_map);
//            //Bind the buffer, draw the depth map and unbind it
//            light_info.shadow_map->bind();
//            //Load de depth light VP into the depth pass
//            light_info.load_depth_vp_matrix(shadow_map_shaders);
//            //Render the depth map
//            draw(false,shadow_map_shaders);
//
//            //Print the depth map
////          light_info.shadow_map->print_in_img((std::string("../depth_map")+std::to_string(i)+std::string(".ppm")).c_str());
//            ShadowMap::unbind_bound_shadow_map();
//
//            depth_maps[count_index_depth_map] = light_info.index_depth_map;
//            lights_struct_array[i].index_sampler_depth_map = count_index_depth_map;
//            count_index_depth_map++;
//        }
//    }
//    m_shaders->use();
//    adapt_viewport();
//
//    for(auto & shadow_map : shadow_maps){
//        shadow_map->activate_texture();
//    }
//
//    glUniform1iv(m_shaders->get_shader_data_manager()->get_location(ShadersDataManager::SHADOW_MAP_ARRAY_LOC_NAME),NB_MAX_LIGHTS, depth_maps);
//
//    m_shaders->get_shader_data_manager()->load_lights(m_shaders->get_program_id(), lights_struct_array,(int)size_lights);
//}
