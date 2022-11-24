//
// Created by mariusjenin on 22/11/22.
//

#ifndef PHOTONEAR_TEXTURE_H
#define PHOTONEAR_TEXTURE_H

#include "glm/glm.hpp"
#include "Transform.h"

using namespace glm;

typedef vec3 color;
namespace texture {
    class Texture {
    public:
        virtual color value(float u, float v, point p) const = 0;
    };
}


#endif //PHOTONEAR_TEXTURE_H
