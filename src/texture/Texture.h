//
// Created by mariusjenin on 22/11/22.
//

#ifndef PHOTONEAR_TEXTURE_H
#define PHOTONEAR_TEXTURE_H

#include "memory"

#include "glm/glm.hpp"
#include "Transform.h"
#include "Shaders.h"
#include "Light.h"

using namespace glm;
using namespace shader_manager;

typedef vec3 color;
namespace texture {
    enum TextureType {
        ColorType = 0,
        TextureType = 1
    };

    class Texture {
    public:
        virtual color value(float u, float v) const = 0;

        virtual void load_in_shaders(const std::shared_ptr<Shaders> &shaders) = 0;

        virtual void generate_ui(const std::string &name) = 0;
    };
}


#endif //PHOTONEAR_TEXTURE_H
