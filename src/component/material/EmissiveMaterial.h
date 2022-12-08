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
            const static int LIGHT_TYPE_DIRECTIONAL = 0;
            const static int LIGHT_TYPE_POINT = 1;
            const static int LIGHT_TYPE_SPOT = 2;

            color
            resolve_ray(SceneGraph *scene_graph, RayTraceHit ray_hit, int depth, void (*on_hit_callback)()) override;
        };
    }
}


#endif //PHOTONEAR_EMISSIVEMATERIAL_H
