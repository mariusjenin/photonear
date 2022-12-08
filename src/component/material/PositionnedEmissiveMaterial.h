//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_POSITIONNEDEMISSIVEMATERIAL_H
#define PHOTONEAR_POSITIONNEDEMISSIVEMATERIAL_H


#include "EmissiveMaterial.h"

namespace component {
    namespace material {
        class PositionnedEmissiveMaterial : public EmissiveMaterial{
        protected:
            float m_linear_attenuation;
            float m_quadratic_attenuation;
        public:
            explicit PositionnedEmissiveMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f), float l_att=0.f, float q_att=0.f);

            Light generate_light() override;

            void generate_ui_component_editor() override;
        };
    }
}


#endif //PHOTONEAR_POSITIONNEDEMISSIVEMATERIAL_H
