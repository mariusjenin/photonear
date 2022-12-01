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
            glm::vec3 m_normal{};
            float m_length_x;
            float m_length_z;

            void assign_mesh_plane();

        public:
            explicit Quad(float length_x, float length_z, bool both_face_visible = false);

            void generate_ui_component_editor() override;

            std::vector<glm::vec3> to_few_vertices() override;
        };
    }
}

#endif //PHOTONEAR_QUAD_H
