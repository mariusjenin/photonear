//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_DIRECTEDLIGHTMATERIAL_H
#define PHOTONEAR_DIRECTEDLIGHTMATERIAL_H



#include "LightMaterial.h"

namespace component{
    namespace material{
        class DirectedLightMaterial : public LightMaterial {
        public:
            explicit DirectedLightMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f));

            Light generate_light() override;
        };
    }
}

#endif //PHOTONEAR_DIRECTEDLIGHTMATERIAL_H
