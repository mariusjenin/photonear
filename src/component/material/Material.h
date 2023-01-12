//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_MATERIAL_H
#define PHOTONEAR_MATERIAL_H


#include "Component.h"
#include "Texture.h"
#include "TextureColor.h"
#include "VertFragShaders.h"
#include "Light.h"
#include "RayCastHit.h"
#include "SceneGraph.h"

using namespace texture;
using namespace shader_manager;
using namespace ray_tracing;
using namespace scene;

namespace component {

    namespace material {
        class Material : public Component {
        protected:
            std::shared_ptr<Texture> m_albedo;
            bool m_emissive;

            explicit Material(std::shared_ptr<Texture> albedo = std::make_shared<TextureColor>(1.0f),bool emissive = false);

            static versor get_direction_reflection(const std::shared_ptr<RayCastHit> &ray_hit);

            static versor get_random_direction_reflection(const std::shared_ptr<RayCastHit> &ray_hit);

            static versor get_scattered_direction_reflection(const std::shared_ptr<RayCastHit> &ray_hit, float roughness);

            static color reflect(SceneGraph *scene_graph, const std::shared_ptr<RayCastHit> &ray_hit, int depth,
                                 versor direction,
                                 color default_color, bool photon_mapping_pass, float factor_weight = 1.f);
        public:
            constexpr const static float REFRACTIVE_INDEX_AIR = 1.000272f;

            virtual void load_in_shaders(const std::shared_ptr<Shaders> &shaders);

            static Material* get_default();

            bool is_emissive() const;

            ComponentType get_type() override;

            void generate_ui_component_editor() override;

            virtual color resolve_ray(SceneGraph *scene_graph, std::shared_ptr<RayCastHit> ray_hit,
                                      int depth, color default_color, bool photon_mapping_pass) = 0;

            static bool discard_ray(const std::shared_ptr<RayCastHit>& ray_hit);
        };
    }
}

#endif //PHOTONEAR_MATERIAL_H
