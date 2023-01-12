//
// Created by mariusjenin on 12/01/23.
//

#ifndef PHOTONEAR_DIFFUSEMATERIAL_H
#define PHOTONEAR_DIFFUSEMATERIAL_H

#include "Material.h"
#include "Photon.h"
#include "RayTraceHit.h"

namespace component {
    namespace material {
        enum DiffuseMaterialType {
            Plastic,
            Metal
        };

        class DiffuseMaterial : public Material {
        protected:
            DiffuseMaterialType m_type;
            float m_roughness{};

        public:
            explicit DiffuseMaterial(DiffuseMaterialType type = Plastic,
                                     std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),
                                     float roughness = 1.f);

            void generate_ui_component_editor() override;

            color resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth,
                              color default_color, bool photon_mapping_pass) override;
            color plastic_resolve_ray(SceneGraph *scene_graph, const std::shared_ptr<RayCastHit>& ray_hit, int depth,
                                      color default_color, bool photon_mapping_pass) const;
            color metal_resolve_ray(SceneGraph *scene_graph, const std::shared_ptr<RayCastHit>& ray_hit, int depth,
                                    color default_color, bool photon_mapping_pass) const;
        };
    }
}


#endif //PHOTONEAR_DIFFUSEMATERIAL_H
