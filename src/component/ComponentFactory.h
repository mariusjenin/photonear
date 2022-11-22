//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_COMPONENTFACTORY_H
#define PHOTONEAR_COMPONENTFACTORY_H

#include "memory"
#include "Sphere.h"
#include "Camera.h"

namespace component {
    class ComponentFactory {
    public:
        static std::shared_ptr<Sphere> create_sphere(glm::vec3 center={0,0,0}, float radius = 1, int slices = 20, int stacks = 20);
        static std::shared_ptr<Camera> create_camera(int priority = 0);
    };
}

#endif //PHOTONEAR_COMPONENTFACTORY_H
