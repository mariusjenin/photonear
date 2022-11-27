//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_DIFFUSEMATERIAL_H
#define PHOTONEAR_DIFFUSEMATERIAL_H

#include "Material.h"

namespace component{
    namespace material{
        class DiffuseMaterial : public Material{
        public:
            explicit DiffuseMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f));
        };
    }
}

#endif //PHOTONEAR_DIFFUSEMATERIAL_H
