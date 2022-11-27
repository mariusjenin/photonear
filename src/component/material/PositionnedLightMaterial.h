//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_POSITIONNEDLIGHTMATERIAL_H
#define PHOTONEAR_POSITIONNEDLIGHTMATERIAL_H


#include "LightMaterial.h"

namespace component {
    namespace material {
        class PositionnedLightMaterial : public LightMaterial{
        protected:
            float m_constant_attenuation;
            float m_linear_attenuation;
            float m_quadratic_attenuation;
        public:
            explicit PositionnedLightMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),
                    float c_att = 0.f,float l_att=0.f,float q_att=0.f);

            Light generate_light() override;
        };
    }
}


#endif //PHOTONEAR_POSITIONNEDLIGHTMATERIAL_H
