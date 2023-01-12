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
            int m_slices;
            int m_stacks;

            static point spheric_to_euclidian_coords(float theta, float phi) {
                return {cos(theta) * cos(phi), sin(theta) * cos(phi), sin(phi)};
            }

            void assign_mesh_sphere();

            std::vector<point> to_few_vertices() override;

        public:
            explicit Sphere(float radius = 1, int slices = 20, int stacks = 20,
                            bool both_face_visible = true);

            void generate_ui_component_editor() override;

            RayCastHit hit(Ray ray) override;

            bool normal_test(std::shared_ptr<RayTraceHit> ray_hit, std::shared_ptr<Photon> photon) override;
        };
    }
}

#endif //PHOTONEAR_SPHERE_H
