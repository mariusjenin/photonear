//
// Created by mariusjenin on 08/12/22.
//

#ifndef PHOTONEAR_DIELECTRICMATERIAL_H
#define PHOTONEAR_DIELECTRICMATERIAL_H

#include "DiffuseMaterial.h"

namespace component{
    namespace material{
        class DielectricMaterial : public DiffuseMaterial{
        protected:
            float m_refractive_index;
        public:
            explicit DielectricMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),float roughness=0.f,float refractive_index = 1.f);

            void generate_ui_component_editor() override;

            virtual void load_in_shaders(const std::shared_ptr<Shaders> &shaders);
        };
    }
}

#endif //PHOTONEAR_DIELECTRICMATERIAL_H
