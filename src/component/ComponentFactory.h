//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_COMPONENTFACTORY_H
#define PHOTONEAR_COMPONENTFACTORY_H

#include "memory"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"

namespace component {

    using namespace shape;

    class ComponentFactory {
    public:
        static std::shared_ptr<Camera> create_camera(int priority = 0);

        static std::shared_ptr<Sphere>
        create_sphere(bool both_side_visible = false, float radius = 1, int slices = 20, int stacks = 20,
                      glm::vec3 center = {0, 0, 0});

        static std::shared_ptr<Triangle> create_triangle(bool both_side_visible = false);
    };
}

#endif //PHOTONEAR_COMPONENTFACTORY_H
