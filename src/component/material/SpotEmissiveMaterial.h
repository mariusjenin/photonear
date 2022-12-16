//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_SPOTEMISSIVEMATERIAL_H
#define PHOTONEAR_SPOTEMISSIVEMATERIAL_H

#include "PositionnedEmissiveMaterial.h"

namespace component {
    namespace material {
        class SpotEmissiveMaterial : public PositionnedEmissiveMaterial{
        private:
            float m_inner_cutoff;
            float m_outer_cutoff;
            float m_inner_cutoff_computed;
            float m_outer_cutoff_computed;
            float m_cut_off_angle;
            float m_bias;
            int m_resolution;
            GLuint m_id_texture_shadow_map{};
            std::shared_ptr<ShadowMap> m_shadow_map;
        public:
            explicit SpotEmissiveMaterial(GLuint id_texture_shadow_map, std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f), float intensity = 1.f,
                                          float in_co=20.f, float out_co = 25.f, int resolution = 1000.f, float bias = 0.001f, float l_att=0.f, float q_att=0.f);

            Light generate_light() override;

            void generate_ui_component_editor() override;

            Ray get_random_ray(glm::mat4 matrix) override;
        };
    }
}

#endif //PHOTONEAR_SPOTEMISSIVEMATERIAL_H
