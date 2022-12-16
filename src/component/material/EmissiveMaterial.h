//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_EMISSIVEMATERIAL_H
#define PHOTONEAR_EMISSIVEMATERIAL_H

#include "Material.h"

namespace component{

    namespace material{
        class EmissiveMaterial : public Material {
        private:
            float m_intensity;
        protected:
            explicit EmissiveMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f), float intensity = 1.f);

        public:
            static float DefaultIntensityForPhotonEmission;

            virtual Light generate_light();

            color
            resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) override;

            virtual Ray get_random_ray(glm::mat4 matrix) = 0;

            void alterate(const std::shared_ptr<RayCastHit>& ray_hit);

            void generate_ui_component_editor() override;
        };
    }
}


#endif //PHOTONEAR_EMISSIVEMATERIAL_H
