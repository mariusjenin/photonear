//
// Created by mariusjenin on 08/12/22.
//

#include "ConductorMaterial.h"

using namespace material;

ConductorMaterial::ConductorMaterial(std::shared_ptr<TextureColor> albedo, float roughness) :
        DiffuseMaterial(MaterialType::MaterialTypeReflective, std::move(albedo), roughness) {}
