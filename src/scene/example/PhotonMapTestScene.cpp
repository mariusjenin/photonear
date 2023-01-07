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
#include "DielectricMaterial.h"
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
    auto back_quad_node = NodeFactory::create_node(quad_node,"BackQuadNode");
    auto down_quad_node = NodeFactory::create_node(quad_node,"DownQuadNode");
    auto left_quad_node = NodeFactory::create_node(quad_node,"LeftQuadNode");
    auto right_quad_node = NodeFactory::create_node(quad_node,"RightQuadNode");
    auto sphere_node = NodeFactory::create_node(root,"SphereNode");
    auto light_node = NodeFactory::create_node(root,"LightNode");

    // Quad
    auto quad_down = make_shared<Quad>(15,15);
    auto quad_back = make_shared<Quad>(15,15);
    auto quad_right = make_shared<Quad>(15,15);
    auto quad_left = make_shared<Quad>(15,15);
    Component::add_component_to_node(quad_down, down_quad_node);
    Component::add_component_to_node(quad_back, back_quad_node);
    Component::add_component_to_node(quad_right, left_quad_node);
    Component::add_component_to_node(quad_left, right_quad_node);
    auto trsf_back_quad= Component::get_component<TransformComponent>(&*back_quad_node)->get_transform();
    trsf_back_quad->set_translation({0,7.5,-7.5});
    trsf_back_quad->set_rotation({90,0,0});
    auto trsf_left_quad= Component::get_component<TransformComponent>(&*left_quad_node)->get_transform();
    trsf_left_quad->set_translation({-7.5,7.5,0});
    trsf_left_quad->set_rotation({0,0,-90});
    auto trsf_right_quad= Component::get_component<TransformComponent>(&*right_quad_node)->get_transform();
    trsf_right_quad->set_translation({7.5,7.5,0});
    trsf_right_quad->set_rotation({0,0,90});

    //Sphere
    auto sphere = make_shared<Sphere>(1.75,40,20);
    Component::add_component_to_node(sphere, sphere_node);
    auto trsf_sphere_node = Component::get_component<TransformComponent>(&*sphere_node)->get_transform();
    trsf_sphere_node->set_translation({0,5,0});

    // Light
    auto point_light = make_shared<PositionnedEmissiveMaterial>(make_shared<TextureColor>(vec3(1, 1, 1)),5.f);
    Component::add_component_to_node(point_light, light_node);
    auto trsf_light_1 = Component::get_component<TransformComponent>(&*light_node)->get_transform();
    trsf_light_1->set_translation({0,10,0});

    // Material
    auto grey_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(0.8f, 0.8f, 0.8f),0.5f);
    auto redish_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(1.f, 0.f, 0.f),0.5f);
    auto greenish_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(0.f, 1.f, 0.f),0.5f);
    auto blueish_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(0.f, 0.f, 1.f),0.5f);
    auto dielectric_material = make_shared<DielectricMaterial>(make_shared<TextureColor>(1.f,1.f,1.f),0.,1.33);
    Component::add_component_to_node(grey_material, down_quad_node);
    Component::add_component_to_node(redish_material, back_quad_node);
    Component::add_component_to_node(greenish_material, left_quad_node);
    Component::add_component_to_node(blueish_material, right_quad_node);
    Component::add_component_to_node(dielectric_material, sphere_node);

    // Camera
    auto camera = make_shared<Camera>();
    Component::add_component_to_node(camera, camera_node);
    auto trsf_camera = Component::get_component<TransformComponent>(&*camera_node)->get_transform();
    trsf_camera->set_translation({0,17,12});
    trsf_camera->set_rotation({-50,0,0});

    m_scene_graph = make_shared<SceneGraph>(root);
}

PhotonMapTestScene::PhotonMapTestScene(GLFWwindow *window, const string &vertex_shader_path,
                     const string &fragment_shader_path) : Scene(window, vertex_shader_path,fragment_shader_path) {}