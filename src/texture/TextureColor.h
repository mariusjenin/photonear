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

        explicit TextureColor(float v);

        color value(float u, float v, point p) const override;

        void load_in_shaders(const std::shared_ptr<Shaders>& shaders) override;

        void load_in_light_shaders(Light* light);

        void generate_ui(const std::string& name) override;

    private:
        color m_value{};
    };
}


#endif //PHOTONEAR_TEXTURECOLOR_H
