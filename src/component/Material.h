//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_MATERIAL_H
#define PHOTONEAR_MATERIAL_H


#include "Component.h"
#include "Texture.h"
#include "TextureColor.h"
#include "VertFragShaders.h"

using namespace texture;
using namespace shader_manager;

namespace component {
    class Material : public Component {
    private:
        // ALBEDO
        std::shared_ptr<Texture> m_albedo;

        // EMISSIVE
        bool m_emissive;

        // METALLIC
        bool m_metallic;
        float m_metallic_fuzz;

        // REFRACTIVE
        bool m_refractive;
        float m_refractive_index;
    public:
        Material(std::shared_ptr<Texture> albedo = std::make_shared<TextureColor>(), bool emissive = false, bool metallic = false, float metallic_fuzz = 0.f, bool refractive = false, float refractive_index = 1.000293f);

        void load_in_shaders(const std::shared_ptr<VertFragShaders>& shaders);

        static std::shared_ptr<Material> get_default();

        ComponentType get_type() override;
    };
}

#endif //PHOTONEAR_MATERIAL_H
