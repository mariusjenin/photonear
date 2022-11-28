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

using namespace texture;
using namespace shader_manager;

namespace component {
    namespace material {
        class Material : public Component {
        protected:
            // ALBEDO
            std::shared_ptr<Texture> m_albedo;

            // EMISSIVE
            bool m_emissive;

            // METALLIC
            bool m_metallic;
            float m_metallic_glossiness;

            // REFRACTIVE
            bool m_refractive;
            float m_refractive_index;

            explicit Material(std::shared_ptr<Texture> albedo = std::make_shared<TextureColor>(1.0f),
                              bool emissive = false, bool metallic = false, float metallic_glossiness = 0.f,
                              bool refractive = false, float refractive_index = 1.000293f);

        public:
            const static int MATERIAL_TYPE_COLOR = 0;
            const static int MATERIAL_TYPE_TEXTURE = 1;

            void load_in_shaders(const std::shared_ptr<Shaders> &shaders);

            static std::shared_ptr<Material> get_default();

            bool is_emissive() const;

            ComponentType get_type() override;

            void generate_component_editor_ui() override;
        };
    }
}

#endif //PHOTONEAR_MATERIAL_H
