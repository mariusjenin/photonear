//
// Created by mariusjenin on 08/12/22.
//

#ifndef PHOTONEAR_CONDUCTORMATERIAL_H
#define PHOTONEAR_CONDUCTORMATERIAL_H

#include "DiffuseMaterial.h"

namespace component{
    namespace material{
        class ConductorMaterial : public DiffuseMaterial{
        public:
            explicit ConductorMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),float roughness=0.f);

            color
            resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) override;
        };
    }
}

#endif //PHOTONEAR_CONDUCTORMATERIAL_H
