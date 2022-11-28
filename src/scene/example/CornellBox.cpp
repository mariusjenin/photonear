//
// Created by mariusjenin on 27/11/22.
//

#include <memory>

#include "CornellBox.h"
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

CornellBox::CornellBox(GLFWwindow *window, const string &vertex_shader_path, const string &fragment_shader_path,
                       vec3 clear_color) : Scene(window, vertex_shader_path, fragment_shader_path, clear_color) {}

void CornellBox::init_scene_graph() {
    auto texture_manager = m_shaders->get_texture_manager();

    //CREATE THE SCENE GRAPH
    auto root = NodeFactory::create_root_node();
    auto camera_node = NodeFactory::create_node(root,"CameraNode");
    auto content_node = NodeFactory::create_node(root,"ContentNode");
    auto light_node = NodeFactory::create_node(root,"LightNode");

    // Wall Node
    auto wall_node = NodeFactory::create_node(root,"WallNode");
    auto wall_front_node = NodeFactory::create_node(wall_node,"WallFrontNode");
    auto wall_back_node = NodeFactory::create_node(wall_node,"WallBackNode");
    auto wall_right_node = NodeFactory::create_node(wall_node,"WallRightNode");
    auto wall_left_node = NodeFactory::create_node(wall_node,"WallLeftNode");
    auto wall_top_node = NodeFactory::create_node(wall_node,"WallTopNode");
    auto wall_bottom_node = NodeFactory::create_node(wall_node,"WallBottomNode");

    // Sphere Node
    auto sphere_1_node = NodeFactory::create_node(content_node);
    auto sphere_2_node = NodeFactory::create_node(content_node);

    // Light Node
    auto light_1_node  = NodeFactory::create_node(light_node);
    auto light_2_node  = NodeFactory::create_node(light_node);
    auto light_3_node  = NodeFactory::create_node(light_node);
    auto light_4_node  = NodeFactory::create_node(light_node);

    // Wall
    auto wall_front = make_shared<Quad>(8,8);
    auto wall_back = make_shared<Quad>(8,8);
    auto wall_left = make_shared<Quad>(8,8);
    auto wall_right = make_shared<Quad>(8,8);
    auto wall_top = make_shared<Quad>(8,8);
    auto wall_bottom = make_shared<Quad>(8,8);
    Component::add_component_to_node(wall_front, wall_front_node);
    Component::add_component_to_node(wall_back, wall_back_node);
    Component::add_component_to_node(wall_left, wall_left_node);
    Component::add_component_to_node(wall_right, wall_right_node);
    Component::add_component_to_node(wall_top, wall_top_node);
    Component::add_component_to_node(wall_bottom, wall_bottom_node);
    auto trsf_wall_front = Component::get_component<TransformComponent>(&*wall_front_node)->get_transform();
    trsf_wall_front->set_translation({0,4,-4}); trsf_wall_front->set_rotation({90,0,0});
    auto trsf_wall_back = Component::get_component<TransformComponent>(&*wall_back_node)->get_transform();
    trsf_wall_back->set_translation({0,4,4}); trsf_wall_back->set_rotation({-90,0,0});
    auto trsf_wall_left = Component::get_component<TransformComponent>(&*wall_left_node)->get_transform();
    trsf_wall_left->set_translation({-4,4,0}); trsf_wall_left->set_rotation({0,0,-90});
    auto trsf_wall_right = Component::get_component<TransformComponent>(&*wall_right_node)->get_transform();
    trsf_wall_right->set_translation({4,4,0}); trsf_wall_right->set_rotation({0,0,90});
    auto trsf_wall_top = Component::get_component<TransformComponent>(&*wall_top_node)->get_transform();
    trsf_wall_top->set_translation({0,8,0}); trsf_wall_top->set_rotation({180,0,0});

    //Sphere
    auto sphere_1 = make_shared<Sphere>(1,40,20);
    auto sphere_2 = make_shared<Sphere>(1.75,80,80);
    Component::add_component_to_node(sphere_1, sphere_1_node);
    Component::add_component_to_node(sphere_2, sphere_2_node);
    auto trsf_sphere_1 = Component::get_component<TransformComponent>(&*sphere_1_node)->get_transform();
    auto trsf_sphere_2 = Component::get_component<TransformComponent>(&*sphere_2_node)->get_transform();
    trsf_sphere_1->set_translation({1.8, 1, 2.8});
    trsf_sphere_2->set_translation({-1.7, 1.75, -1.7});

    // Light
    auto spot_intensity = vec3(0.25,0.25,0.23);
    auto ambient_spot_intensity = vec3(0.08,0.08,0.06);
    auto sphere_light_1 = make_shared<Sphere>(0.1,30,30);
    auto sphere_light_2 = make_shared<Sphere>(0.1,30,30);
    auto sphere_light_3 = make_shared<Sphere>(0.1,30,30);
    auto sphere_light_4 = make_shared<Sphere>(0.1,30,30);
    auto spot_light1 = make_shared<SpotLightMaterial>(texture_manager->get_and_increment_id_texture(), make_shared<TextureColor>(spot_intensity),25,45,700, 0.02f);
    auto spot_light2 = make_shared<SpotLightMaterial>(texture_manager->get_and_increment_id_texture(), make_shared<TextureColor>(spot_intensity),25,45,700, 0.02f);
    auto spot_light3 = make_shared<SpotLightMaterial>(texture_manager->get_and_increment_id_texture(), make_shared<TextureColor>(spot_intensity),25,45,700, 0.02f);
    auto spot_light4 = make_shared<SpotLightMaterial>(texture_manager->get_and_increment_id_texture(), make_shared<TextureColor>(spot_intensity),25,45,700, 0.02f);
    auto point_light = make_shared<PositionnedLightMaterial>(make_shared<TextureColor>(ambient_spot_intensity));
    auto point_light2 = make_shared<PositionnedLightMaterial>(make_shared<TextureColor>(ambient_spot_intensity));
    auto point_light3 = make_shared<PositionnedLightMaterial>(make_shared<TextureColor>(ambient_spot_intensity));
    auto point_light4 = make_shared<PositionnedLightMaterial>(make_shared<TextureColor>(ambient_spot_intensity));
    Component::add_component_to_node(spot_light1, light_1_node);
    Component::add_component_to_node(point_light, light_1_node);
    Component::add_component_to_node(spot_light2, light_2_node);
    Component::add_component_to_node(point_light2, light_2_node);
    Component::add_component_to_node(spot_light3, light_3_node);
    Component::add_component_to_node(point_light3, light_3_node);
    Component::add_component_to_node(spot_light4, light_4_node);
    Component::add_component_to_node(point_light4, light_4_node);
    auto trsf_light_1 = Component::get_component<TransformComponent>(&*light_1_node)->get_transform();
    trsf_light_1->set_translation({-2.5,7.8,-2.5});
    trsf_light_1->set_rotation({-90,0,0});
    auto trsf_light_2 = Component::get_component<TransformComponent>(&*light_2_node)->get_transform();
    trsf_light_2->set_translation({-2.5,7.8,2.5});
    trsf_light_2->set_rotation({-90,0,0});
    auto trsf_light_3 = Component::get_component<TransformComponent>(&*light_3_node)->get_transform();
    trsf_light_3->set_translation({2.5,7.8,-2.5});
    trsf_light_3->set_rotation({-90,0,0});
    auto trsf_light_4 = Component::get_component<TransformComponent>(&*light_4_node)->get_transform();
    trsf_light_4->set_translation({2.5,7.8,2.5});
    trsf_light_4->set_rotation({-90,0,0});

    Component::add_component_to_node(sphere_light_1, light_1_node);
    Component::add_component_to_node(sphere_light_2, light_2_node);
    Component::add_component_to_node(sphere_light_3, light_3_node);
    Component::add_component_to_node(sphere_light_4, light_4_node);


    // Material
    auto grey_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(0.9f,0.9f,0.9f));
    auto red_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(0.85f,0.25f,0.25f));
    auto green_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(0.18f,0.79f,0.02f));
    auto cyan_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(0.f,1.f,1.f));
    auto magenta_material = make_shared<DiffuseMaterial>(make_shared<TextureColor>(1.f,0.f,1.f));
    Component::add_component_to_node(grey_material, wall_node);
    Component::add_component_to_node(red_material, wall_left_node);
    Component::add_component_to_node(green_material, wall_right_node);
    Component::add_component_to_node(cyan_material, sphere_1_node);
    Component::add_component_to_node(magenta_material, sphere_2_node);


    // Camera
    auto camera = make_shared<Camera>();
    Component::add_component_to_node(camera, camera_node);
    auto trsf_camera = Component::get_component<TransformComponent>(&*camera_node)->get_transform();
    trsf_camera->set_translation({0,8.8,12});
    trsf_camera->set_rotation({-25,0,0});

    m_scene_graph = make_shared<SceneGraph>(root);
}

