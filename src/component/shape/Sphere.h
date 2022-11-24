//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SPHERE_H
#define PHOTONEAR_SPHERE_H

#include "glm/glm.hpp"
#include "Shape.h"

using namespace std;
using namespace glm;

namespace component {
    namespace shape {
        class Sphere : public Shape {
        private:
            point m_center{};
            float m_radius;

            static glm::vec3 spheric_to_euclidian_coords(float theta, float phi) {
                return {cos(theta) * cos(phi), sin(theta) * cos(phi), sin(phi)};
            }

            void assign_mesh_sphere(float radius, int slices, int stacks);

        public:
            explicit Sphere(bool both_face_visible = false, float radius = 1, int slices = 20, int stacks = 20,
                            point center = {0, 0, 0});
        };
    }
}

#endif //PHOTONEAR_SPHERE_H
