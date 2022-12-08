//
// Created by mariusjenin on 22/11/22.
//

#include "include_stbi.h"
#include "TextureImage.h"

using namespace texture;

color TextureImage::value(float u, float v) const  {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (m_data == nullptr)
        return {0, 1, 1};

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = clamp(u, 0.0f, 1.0f);
    v = 1.0f - clamp(v, 0.0f, 1.0f);  // Flip V to image coordinates

    auto i = static_cast<int>(u * (float)m_width);
    auto j = static_cast<int>(v * (float)m_height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= m_width) i = m_width - 1;
    if (j >= m_height) j = m_height - 1;

    const auto color_scale = 1.0 / 255.0;
    auto pixel = m_data + j * m_bytes_per_scanline + i * bytes_per_pixel;

    return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
}

TextureImage::TextureImage() {
    m_data = nullptr;
    m_width = 0;
    m_height = 0;
    m_bytes_per_scanline = 0;
}

TextureImage::TextureImage(const char *filename) {
    auto components_per_pixel = bytes_per_pixel;

    m_data = stbi_load(filename, &m_width, &m_height, &components_per_pixel, components_per_pixel);

    if (!m_data) {
        std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
        m_width = m_height = 0;
    }

    m_bytes_per_scanline = bytes_per_pixel * m_width;
}
