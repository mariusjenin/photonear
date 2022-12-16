//
// Created by mariusjenin on 12/12/22.
//


#include <memory>

#include "PhotonMapTestScene.h"
#include "NodeFactory.h"
#include "Node.h"
#include "Camera.h"
#include "DiffuseMaterial.h"
#include "ConductorMaterial.h"
#include "PositionnedEmissiveMaterial.h"
#include "TextureManager.h"
#include "Sphere.h"
#include "Quad.h"

using namespace std;
using namespace scene;
using namespace scene::node;
using namespace component::material;
using namespace component::shape;

void PhotonMapTestScene::init_scene_graph() {
    //CREATE THE SCENE GRAPH
    auto root = NodeFactory::create_root_node();
    auto camera_node = NodeFactory::create_node(root,"CameraNode");
    auto quad_node = NodeFactory::create_node(root,"QuadNode");
    auto light_node = NodeFactory::create_node(root,"LightNode");
    auto light_node_node = NodeFactory::create_node(light_node,"LightNodeNode");

    // Quad
    auto quad = make_shared<Quad>(10,6);
    Component::add_component_to_node(quad, quad_node);
    auto trsf_wall_back = Component::get_component<TransformComponent>(&*quad_node)->get_transform();
    trsf_wall_back->set_translation({0,4,0});


    // Light
    auto ambient_spot_intensity = vec3(0.08,0.08,0.06);
    auto point_light = make_shared<PositionnedEmissiveMaterial>(make_shared<TextureColor>(vec3(0.8, 0.8, 0.75)));
    Component::add_component_to_node(point_light, light_node_node);
    auto trsf_light_1 = Component::get_component<TransformComponent>(&*light_node)->get_transform();
    trsf_light_1->set_translation({0,7.8,0});

    // Material
    auto red_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(1.f,0.f,0.f));
//    auto red_material = make_shared<ConductorMaterial>(make_shared<TextureColor>(1.f,0.f,0.f),0.f);
    Component::add_component_to_node(red_material, quad_node);

    // Camera
    auto camera = make_shared<Camera>();
    Component::add_component_to_node(camera, camera_node);
    auto trsf_camera = Component::get_component<TransformComponent>(&*camera_node)->get_transform();
    trsf_camera->set_translation({0,14,17});
    trsf_camera->set_rotation({-30,0,0});

    m_scene_graph = make_shared<SceneGraph>(root);
}

PhotonMapTestScene::PhotonMapTestScene(GLFWwindow *window, const string &vertex_shader_path,
                     const string &fragment_shader_path) : Scene(window, vertex_shader_path,fragment_shader_path) {}