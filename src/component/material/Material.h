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
#include "RayTraceHit.h"
#include "SceneGraph.h"

using namespace texture;
using namespace shader_manager;
using namespace ray_tracing;
using namespace scene;

namespace component {
    enum MaterialType{
        MaterialTypeEmissive,
        MaterialTypeDiffuse,
        MaterialTypeReflective,
        MaterialTypeRefractive
    };

    namespace material {
        class Material : public Component {
        protected:
            std::shared_ptr<Texture> m_albedo;
            MaterialType m_type;

            explicit Material(MaterialType type, std::shared_ptr<Texture> albedo = std::make_shared<TextureColor>(1.0f));

        public:

            virtual void load_in_shaders(const std::shared_ptr<Shaders> &shaders);

            static std::shared_ptr<Material> get_default();

            bool is_emissive() const;

            ComponentType get_type() override;

            void generate_ui_component_editor() override;

            virtual color resolve_ray(SceneGraph *scene_graph, RayTraceHit ray_hit,
                                      int depth, void (*on_hit_callback)()) = 0;
        };
    }
}

#endif //PHOTONEAR_MATERIAL_H
