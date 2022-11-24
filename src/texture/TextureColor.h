//
// Created by mariusjenin on 22/11/22.
//

#ifndef PHOTONEAR_TEXTURECOLOR_H
#define PHOTONEAR_TEXTURECOLOR_H

#include "Texture.h"

namespace texture {
    class TextureColor : public Texture {
    public:
        TextureColor();

        explicit TextureColor(color c);

        TextureColor(float r, float g, float b);

        color value(float u, float v, point p) const override;

    private:
        color m_value{};
    };
}


#endif //PHOTONEAR_TEXTURECOLOR_H
