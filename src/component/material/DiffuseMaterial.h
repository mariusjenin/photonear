//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_DIFFUSEMATERIAL_H
#define PHOTONEAR_DIFFUSEMATERIAL_H

#include "Material.h"

namespace component {
    namespace material {
        class DiffuseMaterial : public Material {
        protected:
            float m_roughness{};

            explicit DiffuseMaterial(MaterialType type,
                                     std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),
                                     float roughness = 0.f);

        public:
            explicit DiffuseMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f),
                                     float roughness = 0.f);

            void generate_ui_component_editor() override;

            color
            resolve_ray(SceneGraph *scene_graph, RayTraceHit ray_hit, int depth, void (*on_hit_callback)()) override;
        };
    }
}

#endif //PHOTONEAR_DIFFUSEMATERIAL_H
