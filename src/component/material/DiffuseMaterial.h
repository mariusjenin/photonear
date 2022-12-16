//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_DIFFUSEMATERIAL_H
#define PHOTONEAR_DIFFUSEMATERIAL_H

#include "Material.h"
#include "Photon.h"
#include "RayTraceHit.h"

namespace component {
    namespace material {
        class DiffuseMaterial : public Material {
        protected:
            float m_roughness{};

            explicit DiffuseMaterial(MaterialType type,
                                     std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),
                                     float roughness = 0.f);

            versor get_direction_reflection(const std::shared_ptr<RayCastHit>& ray_hit);

        public:
            explicit DiffuseMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),
                                     float roughness = 0.f);

            void generate_ui_component_editor() override;

            color
            resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit, int depth, color default_color, bool photon_mapping_pass) override;

            float oren_nayar_brdf(const std::shared_ptr<RayTraceHit>& ray_trace_hit, const std::shared_ptr<Photon>& photon) const;
        };
    }
}

#endif //PHOTONEAR_DIFFUSEMATERIAL_H
