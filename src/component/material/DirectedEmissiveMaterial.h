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
            explicit DirectedEmissiveMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f), float intensity = 1.f);

            Light generate_light() override;

            Ray get_random_ray(glm::mat4 matrix) override;
        };
    }
}

#endif //PHOTONEAR_DIRECTEDEMISSIVEMATERIAL_H
