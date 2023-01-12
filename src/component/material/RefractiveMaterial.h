//
// Created by mariusjenin on 08/12/22.
//

#ifndef PHOTONEAR_REFRACTIVEMATERIAL_H
#define PHOTONEAR_REFRACTIVEMATERIAL_H

#include "DiffuseMaterial.h"

namespace component{
    namespace material{
        class RefractiveMaterial : public Material{
        private:
            color refract(SceneGraph *scene_graph,const std::shared_ptr<RayCastHit>& ray_hit, int depth,
                          color default_color, bool photon_mapping_pass,float factor_weight = 1.0f);
        protected:
            float m_refractive_index;
        public:
            explicit RefractiveMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f), float refractive_index = 1.f);

            void generate_ui_component_editor() override;

            color
            resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) override;
        };
    }
}

#endif //PHOTONEAR_REFRACTIVEMATERIAL_H
