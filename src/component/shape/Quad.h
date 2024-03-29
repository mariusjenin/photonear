//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_QUAD_H
#define PHOTONEAR_QUAD_H

#include "glm/glm.hpp"
#include "Shape.h"

using namespace std;
using namespace glm;

namespace component {
    namespace shape {
        class Quad : public Shape {
        private:
            versor m_normal{};
            float m_length_x;
            float m_length_z;
            float m_accepted_normal_angle;

            void assign_mesh_plane();

        public:
            explicit Quad(float length_x, float length_z, bool both_face_visible = true,float accepted_normal_angle = 15.f);

            void generate_ui_component_editor() override;

            std::vector<point> to_few_vertices() override;

            RayCastHit hit(Ray ray) override;

            bool normal_test(std::shared_ptr<RayTraceHit> ray_hit, std::shared_ptr<Photon> photon) override;
        };
    }
}

#endif //PHOTONEAR_QUAD_H
