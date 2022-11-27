//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_SPOTLIGHTMATERIAL_H
#define PHOTONEAR_SPOTLIGHTMATERIAL_H

#include "PositionnedLightMaterial.h"

namespace component {
    namespace material {
        class SpotLightMaterial : public PositionnedLightMaterial{
        private:
            float m_inner_cutoff;
            float m_outer_cutoff;
            float m_cut_off_angle;
            float m_bias;
            int m_resolution;
            GLuint m_id_texture_shadow_map{};
            std::shared_ptr<ShadowMap> m_shadow_map;
        public:
            explicit SpotLightMaterial( GLuint id_texture_shadow_map, std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),
                                       float in_co=20.f, float out_co = 25.f, int resolution = 1000.f,float bias = 0.001f,
                                       float c_att=0.f,float l_att=0.f,float q_att=0.f);

            Light generate_light() override;
        };
    }
}

#endif //PHOTONEAR_SPOTLIGHTMATERIAL_H
