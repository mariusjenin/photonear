//
// Created by mariusjenin on 27/11/22.
//

#include <memory>

#include "CornellBox.h"
#include "NodeFactory.h"
#include "Node.h"
#include "Camera.h"
#include "PositionnedEmissiveMaterial.h"
#include "SpotEmissiveMaterial.h"
#include "TextureManager.h"
#include "Sphere.h"
#include "Quad.h"
#include "RefractiveMaterial.h"
#include "DiffuseMaterial.h"

using namespace std;
using namespace scene;
using namespace scene::node;
using namespace component::material;
using namespace component::shape;

CornellBox::CornellBox(GLFWwindow *window, const string &vertex_shader_path, const string &fragment_shader_path) : Scene(window, vertex_shader_path, fragment_shader_path) {}

void CornellBox::init_scene_graph() {
    auto texture_manager = m_shaders->get_texture_manager();

    //CREATE THE SCENE GRAPH
    auto root = NodeFactory::create_root_node();
    auto camera_node = NodeFactory::create_node(root,"CameraNode");
    auto content_node = NodeFactory::create_node(root,"ContentNode");
    auto light_node = NodeFactory::create_node(root,"LightNode");

    // Wall Node
    auto wall_node = NodeFactory::create_node(root,"WallNode");
    auto wall_back_node = NodeFactory::create_node(wall_node, "WallBackNode");
    auto wall_front_node = NodeFactory::create_node(wall_node, "WallFrontNode");
    auto wall_right_node = NodeFactory::create_node(wall_node,"WallRightNode");
    auto wall_left_node = NodeFactory::create_node(wall_node,"WallLeftNode");
    auto wall_top_node = NodeFactory::create_node(wall_node,"WallTopNode");
    auto wall_bottom_node = NodeFactory::create_node(wall_node,"WallBottomNode");

    // Sphere Node
    auto sphere_1_node = NodeFactory::create_node(content_node, "Sphere1");
    auto sphere_2_node = NodeFactory::create_node(content_node, "Sphere2");

    // Light Node
    auto light_1_node  = NodeFactory::create_node(light_node, "SpotLightNode1");
    auto light_2_node  = NodeFactory::create_node(light_node, "SpotLightNode2");
    auto light_3_node  = NodeFactory::create_node(light_node, "SpotLightNode3");
    auto light_4_node  = NodeFactory::create_node(light_node, "SpotLightNode4");

    // Wall
    auto wall_back = make_shared<Quad>(8, 8);
    auto wall_front = make_shared<Quad>(8, 8, false);
    auto wall_left = make_shared<Quad>(8,8);
    auto wall_right = make_shared<Quad>(8,8);
    auto wall_top = make_shared<Quad>(8,8);
    auto wall_bottom = make_shared<Quad>(8,8);
    Component::add_component_to_node(wall_back, wall_back_node);
    Component::add_component_to_node(wall_front, wall_front_node);
    Component::add_component_to_node(wall_left, wall_left_node);
    Component::add_component_to_node(wall_right, wall_right_node);
    Component::add_component_to_node(wall_top, wall_top_node);
    Component::add_component_to_node(wall_bottom, wall_bottom_node);
    auto trsf_wall_front = Component::get_component<TransformComponent>(&*wall_back_node)->get_transform();
    trsf_wall_front->set_translation({0,4,-4}); trsf_wall_front->set_rotation({90,0,0});
    auto trsf_wall_back = Component::get_component<TransformComponent>(&*wall_front_node)->get_transform();
    trsf_wall_back->set_translation({0,4,4}); trsf_wall_back->set_rotation({-90,0,0});
    auto trsf_wall_left = Component::get_component<TransformComponent>(&*wall_left_node)->get_transform();
    trsf_wall_left->set_translation({-4,4,0}); trsf_wall_left->set_rotation({0,0,-90});
    auto trsf_wall_right = Component::get_component<TransformComponent>(&*wall_right_node)->get_transform();
    trsf_wall_right->set_translation({4,4,0}); trsf_wall_right->set_rotation({0,0,90});
    auto trsf_wall_top = Component::get_component<TransformComponent>(&*wall_top_node)->get_transform();
    trsf_wall_top->set_translation({0,8,0}); trsf_wall_top->set_rotation({180,0,0});

    //Sphere
    auto sphere_2 = make_shared<Sphere>(1, 40, 20);
    auto sphere_1 = make_shared<Sphere>(1.75, 80, 80);
    Component::add_component_to_node(sphere_1, sphere_1_node);
    Component::add_component_to_node(sphere_2, sphere_2_node);
    auto trsf_sphere_1 = Component::get_component<TransformComponent>(&*sphere_1_node)->get_transform();
    auto trsf_sphere_2 = Component::get_component<TransformComponent>(&*sphere_2_node)->get_transform();
    trsf_sphere_1->set_translation({-1.7, 1.75, -1.7});
    trsf_sphere_2->set_translation({1.8, 1, 2.8});

    // Light
    auto light_color = vec3(1.f,1.f,0.98f);
    auto spot_intensity = 0.3f;
    auto inner_cutoff_spot = 45;
    auto outer_cutoff_spot = 50;
    auto sphere_light_1 = make_shared<Sphere>(0.1,30,30);
    auto sphere_light_2 = make_shared<Sphere>(0.1,30,30);
    auto sphere_light_3 = make_shared<Sphere>(0.1,30,30);
    auto sphere_light_4 = make_shared<Sphere>(0.1,30,30);
    auto spot_light1 = make_shared<SpotEmissiveMaterial>(texture_manager->get_and_increment_id_texture(), make_shared<TextureColor>(light_color), spot_intensity, inner_cutoff_spot, outer_cutoff_spot, 700, 0.02f, 0.f, 0.f);
    auto spot_light2 = make_shared<SpotEmissiveMaterial>(texture_manager->get_and_increment_id_texture(), make_shared<TextureColor>(light_color), spot_intensity, inner_cutoff_spot, outer_cutoff_spot, 700, 0.02f, 0.f, 0.f);
    auto spot_light3 = make_shared<SpotEmissiveMaterial>(texture_manager->get_and_increment_id_texture(), make_shared<TextureColor>(light_color), spot_intensity, inner_cutoff_spot, outer_cutoff_spot, 700, 0.02f, 0.f, 0.f);
    auto spot_light4 = make_shared<SpotEmissiveMaterial>(texture_manager->get_and_increment_id_texture(), make_shared<TextureColor>(light_color), spot_intensity, inner_cutoff_spot, outer_cutoff_spot, 700, 0.02f, 0.f, 0.f);
    Component::add_component_to_node(spot_light1, light_1_node);
    Component::add_component_to_node(spot_light2, light_2_node);
    Component::add_component_to_node(spot_light3, light_3_node);
    Component::add_component_to_node(spot_light4, light_4_node);
    auto trsf_light_1 = Component::get_component<TransformComponent>(&*light_1_node)->get_transform();
    trsf_light_1->set_translation({-2.5,7.95,-2.5});
//    trsf_light_1->set_translation({0,4,0});
    trsf_light_1->set_rotation({-90,0,0});
    auto trsf_light_2 = Component::get_component<TransformComponent>(&*light_2_node)->get_transform();
    trsf_light_2->set_translation({-2.5,7.95,2.5});
    trsf_light_2->set_rotation({-90,0,0});
    auto trsf_light_3 = Component::get_component<TransformComponent>(&*light_3_node)->get_transform();
    trsf_light_3->set_translation({2.5,7.95,-2.5});
    trsf_light_3->set_rotation({-90,0,0});
    auto trsf_light_4 = Component::get_component<TransformComponent>(&*light_4_node)->get_transform();
    trsf_light_4->set_translation({2.5,7.95,2.5});
    trsf_light_4->set_rotation({-90,0,0});

//    Component::add_component_to_node(sphere_light_1, light_1_node);
//    Component::add_component_to_node(sphere_light_2, light_2_node);
//    Component::add_component_to_node(sphere_light_3, light_3_node);
//    Component::add_component_to_node(sphere_light_4, light_4_node);


    // Material
    auto grey_material = make_shared<DiffuseMaterial>(DiffuseMaterialType::Plastic,make_shared<TextureColor>(0.9f, 0.9f, 0.9f), 1.f);
//    auto red_material = make_shared<DiffMaterial>(make_shared<TextureColor>(1.f, 0.5f, 0.5f), 1.f);
//    auto green_material = make_shared<DiffMaterial>(make_shared<TextureColor>(0.5f, 1.f, 0.5f), 1.f);
    auto red_material = make_shared<DiffuseMaterial>(DiffuseMaterialType::Metal,make_shared<TextureColor>(1.f, 0.5f, 0.5f), 1.f);
    auto green_material = make_shared<DiffuseMaterial>(DiffuseMaterialType::Plastic,make_shared<TextureColor>(0.5f, 1.f, 0.5f), 1.f);
    auto dielectric_material = make_shared<RefractiveMaterial>(make_shared<TextureColor>(1.f, 1.f, 1.f), 1.5f);
    auto conductor_material = make_shared<DiffuseMaterial>(DiffuseMaterialType::Metal,make_shared<TextureColor>(1.f, 1.f, 1.f), 0.f);
    Component::add_component_to_node(grey_material, wall_node);
    Component::add_component_to_node(red_material, wall_left_node);
    Component::add_component_to_node(green_material, wall_right_node);
    Component::add_component_to_node(conductor_material, sphere_1_node);
    Component::add_component_to_node(dielectric_material, sphere_2_node);


    // Camera
    auto camera = make_shared<Camera>();
    auto camera2 = make_shared<Camera>();
    auto camera_node_2 = NodeFactory::create_node(camera_node,"CameraNode2");
    Component::add_component_to_node(camera, camera_node);
    Component::add_component_to_node(camera2, camera_node_2);
    auto trsf_camera = Component::get_component<TransformComponent>(&*camera_node)->get_transform();
    trsf_camera->set_translation({0.001,4,14});
    trsf_camera->set_rotation({0,0,0});
    auto trsf_camera2 = Component::get_component<TransformComponent>(&*camera_node_2)->get_transform();
    trsf_camera2->set_translation({-2.2,-2.6,-5});
    trsf_camera2->set_rotation({10,-17,0});

    m_scene_graph = make_shared<SceneGraph>(root);
}

