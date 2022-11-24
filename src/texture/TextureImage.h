//
// Created by mariusjenin on 22/11/22.
//

#ifndef PHOTONEAR_TEXTUREIMAGE_H
#define PHOTONEAR_TEXTUREIMAGE_H


#include "Texture.h"

namespace texture {
    class TextureImage : public Texture {
    private:
        unsigned char *m_data;
        int m_width;
        int m_height;
        int m_bytes_per_scanline;

    public:
        const static int bytes_per_pixel = 3;

        TextureImage();

        explicit TextureImage(const char *filename);

        color value(float u, float v, point p) const override;

        ~TextureImage() {
            delete m_data;
        }
    };
}


#endif //PHOTONEAR_TEXTUREIMAGE_H
