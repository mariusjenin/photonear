//
// Created by mariusjenin on 27/11/22.
//

#include <memory>

#include "DepthScene.h"
#include "NodeFactory.h"
#include "Node.h"
#include "Camera.h"
#include "DiffuseMaterial.h"
#include "PositionnedLightMaterial.h"
#include "SpotLightMaterial.h"
#include "TextureManager.h"
#include "Sphere.h"
#include "Quad.h"

using namespace std;
using namespace scene;
using namespace scene::node;
using namespace component::material;
using namespace component::shape;

void DepthScene::init_scene_graph() {
    auto texture_manager = m_shaders->get_texture_manager();

    //CREATE THE SCENE GRAPH
    auto root = NodeFactory::create_root_node();
    auto root_2 = NodeFactory::create_node(root,"RootNode2");
    auto camera_node = NodeFactory::create_node(root_2,"CameraNode");
    auto light_node = NodeFactory::create_node(root_2,"LightNode");
    auto light_node_node = NodeFactory::create_node(light_node,"LightNodeNode");

    // Wall Node
    auto wall_node = NodeFactory::create_node(root_2,"WallNode");
    auto wall_node_node = NodeFactory::create_node(wall_node,"WallNodeNode");
    auto wall_back_node = NodeFactory::create_node(wall_node_node,"WallBackNode");
    auto wall_bottom_node = NodeFactory::create_node(wall_node_node,"WallBottomNode");

    // Wall
    auto wall_back = make_shared<Quad>(8,8);
    auto wall_bottom = make_shared<Quad>(8,8);
    Component::add_component_to_node(wall_back, wall_back_node);
    Component::add_component_to_node(wall_bottom, wall_bottom_node);
    auto trsf_wall_back = Component::get_component<TransformComponent>(&*wall_back_node)->get_transform();
    trsf_wall_back->set_translation({0,4,-4}); trsf_wall_back->set_rotation({90,0,0});

    // Light
    auto spot_intensity = vec3(0.4,0.4,0.35);
    auto ambient_spot_intensity = vec3(0.08,0.08,0.06);
    auto sphere_light = make_shared<Sphere>(0.1,30,30);
    auto point_light = make_shared<PositionnedLightMaterial>(make_shared<TextureColor>(vec3(0.8,0.8,0.75)));
    Component::add_component_to_node(point_light, light_node);
    auto trsf_light_1 = Component::get_component<TransformComponent>(&*light_node)->get_transform();
    trsf_light_1->set_translation({0,7.8,0});
    trsf_light_1->set_rotation({-90,0,0});

    Component::add_component_to_node(sphere_light, light_node);

    // Material
    auto grey_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(0.9f,0.9f,0.9f));
    auto red_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(1.f,0.f,0.f));
    Component::add_component_to_node(grey_material, wall_node);
    Component::add_component_to_node(red_material, wall_back_node);

    // Camera
    auto camera = make_shared<Camera>();
    Component::add_component_to_node(camera, camera_node);
    auto trsf_camera = Component::get_component<TransformComponent>(&*camera_node)->get_transform();
    trsf_camera->set_translation({0,8.8,12});
    trsf_camera->set_rotation({-25,0,0});

    m_scene_graph = make_shared<SceneGraph>(root);
}

DepthScene::DepthScene(GLFWwindow *window, const string &vertex_shader_path,
                       const string &fragment_shader_path, vec3 clear_color) : Scene(window,
                                                                                                      vertex_shader_path,
                                                                                                      fragment_shader_path,
                                                                                                      clear_color) {

}

