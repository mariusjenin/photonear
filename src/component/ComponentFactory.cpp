//
// Created by mariusjenin on 19/11/22.
//

#include "ComponentFactory.h"
#include "TransformComponent.h"

using namespace component;

std::shared_ptr<Sphere> ComponentFactory::create_sphere(bool both_side_visible, float radius, int slices, int stacks, glm::vec3 center) {
    return std::make_shared<Sphere>(both_side_visible,radius,slices,stacks,center);
}


std::shared_ptr<Triangle> ComponentFactory::create_triangle(bool both_side_visible) {
    return std::make_shared<Triangle>(both_side_visible);
}


std::shared_ptr<Camera> ComponentFactory::create_camera(int priority) {
    return std::make_shared<Camera>(priority);
}
