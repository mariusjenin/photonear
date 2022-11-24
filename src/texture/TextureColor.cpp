//
// Created by mariusjenin on 22/11/22.
//

#include "TextureColor.h"

using namespace texture;

color TextureColor::value(float u, float v, point p) const {
    return m_value;
}

TextureColor::TextureColor() {
    m_value = color();
}

TextureColor::TextureColor(color c) {
    m_value = c;
}

TextureColor::TextureColor(float r, float g, float b) {
    m_value = {r,g,b};
}
