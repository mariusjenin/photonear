//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_TRIANGLE_H
#define PHOTONEAR_TRIANGLE_H

#include "glm/glm.hpp"
#include "Shape.h"

using namespace std;
using namespace glm;

namespace component {
    namespace shape {
        class Triangle : public Shape {
        private:
            void assign_mesh_triangle();

        public:
            explicit Triangle(bool both_face_visible = false);
        };
    }
}

#endif //PHOTONEAR_TRIANGLE_H
