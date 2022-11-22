//
// Created by mariusjenin on 19/11/22.
//

#include "ComponentFactory.h"

using namespace component;

std::shared_ptr<Sphere> ComponentFactory::create_sphere(glm::vec3 center, float radius, int slices, int stacks) {
    return std::make_shared<Sphere>(center,radius,slices,stacks);
}


std::shared_ptr<Camera> ComponentFactory::create_camera(int priority) {
    return std::make_shared<Camera>(priority);
}
