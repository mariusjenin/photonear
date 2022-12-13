//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_EMISSIVEMATERIAL_H
#define PHOTONEAR_EMISSIVEMATERIAL_H

#include "Material.h"

namespace component{

    namespace material{
        class EmissiveMaterial : public Material {
        protected:
            explicit EmissiveMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f));

        public:
            virtual Light generate_light();

            color
            resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) override;

            virtual Ray get_random_ray(glm::mat4 matrix) = 0;
        };
    }
}


#endif //PHOTONEAR_EMISSIVEMATERIAL_H
