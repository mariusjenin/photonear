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
            glm::vec3 m_points[4]{};
            void assign_mesh_plane();

        public:
            explicit Quad(glm::vec3 top_left_pt = {-1, -1, -1}, glm::vec3 bot_left_pt = {-1, -1, 1},
                          glm::vec3 bot_right_pt = {1, -1, 1}, bool both_face_visible = false);
        };
    }
}

#endif //PHOTONEAR_QUAD_H
