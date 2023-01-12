//
// Created by mariusjenin on 27/11/22.
//

#include <memory>

#include "QuadScene.h"
#include "NodeFactory.h"
#include "Node.h"
#include "Camera.h"
#include "DiffuseMaterial.h"
#include "PositionnedEmissiveMaterial.h"
#include "TextureManager.h"
#include "Sphere.h"
#include "Quad.h"

using namespace std;
using namespace scene;
using namespace scene::node;
using namespace component::material;
using namespace component::shape;

void QuadScene::init_scene_graph() {
    //CREATE THE SCENE GRAPH
    auto root = NodeFactory::create_root_node();
    auto quad_node = NodeFactory::create_node(root,"QuadNode");
    auto sphere_node = NodeFactory::create_node(root,"SphereNode");
    auto camera_node = NodeFactory::create_node(root,"CameraNode");
    auto light_node = NodeFactory::create_node(root,"LightNode");

    // Quad
    auto quad = make_shared<Quad>(10,6);
    Component::add_component_to_node(quad, quad_node);
    auto trsf_wall_back = Component::get_component<TransformComponent>(&*quad_node)->get_transform();
    trsf_wall_back->set_translation({0,4,0});

    //Sphere
    auto sphere = make_shared<Sphere>(1,50,50);
    Component::add_component_to_node(sphere, sphere_node);
    auto trsf_sphere = Component::get_component<TransformComponent>(&*sphere_node)->get_transform();
    trsf_sphere->set_translation({0,6,0});

    // Light
    auto ambient_spot_intensity = vec3(0.08,0.08,0.06);
//    auto sphere_light = make_shared<Sphere>(0.1,30,30);
    auto point_light = make_shared<PositionnedEmissiveMaterial>(make_shared<TextureColor>(vec3(0.8, 0.8, 0.75)));
    Component::add_component_to_node(point_light, light_node);
    auto trsf_light_1 = Component::get_component<TransformComponent>(&*light_node)->get_transform();
    trsf_light_1->set_translation({0,8,0});
//    Component::add_component_to_node(sphere_light, light_node);

    // Material
    auto red_material = make_shared<DiffuseMaterial>(DiffuseMaterialType::Plastic,make_shared<TextureColor>(1.f, 0.f, 0.f));
    Component::add_component_to_node(red_material, quad_node);

    // Camera
    auto camera = make_shared<Camera>();
    Component::add_component_to_node(camera, camera_node);
    auto trsf_camera = Component::get_component<TransformComponent>(&*camera_node)->get_transform();
    trsf_camera->set_translation({0,14,17});
    trsf_camera->set_rotation({-30,0,0});

    m_scene_graph = make_shared<SceneGraph>(root);
}

QuadScene::QuadScene(GLFWwindow *window, const string &vertex_shader_path,
                     const string &fragment_shader_path) : Scene(window, vertex_shader_path,fragment_shader_path) {}

