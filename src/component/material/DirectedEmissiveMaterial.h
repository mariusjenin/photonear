//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_DIRECTEDEMISSIVEMATERIAL_H
#define PHOTONEAR_DIRECTEDEMISSIVEMATERIAL_H



#include "EmissiveMaterial.h"

namespace component{
    namespace material{
        class DirectedEmissiveMaterial : public EmissiveMaterial {
        public:
            explicit DirectedEmissiveMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f));

            Light generate_light() override;
        };
    }
}

#endif //PHOTONEAR_DIRECTEDEMISSIVEMATERIAL_H
