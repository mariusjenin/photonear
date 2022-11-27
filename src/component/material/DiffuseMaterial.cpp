//
// Created by mariusjenin on 24/11/22.
//

#include "DiffuseMaterial.h"

#include <utility>

using namespace component::material;

DiffuseMaterial::DiffuseMaterial(std::shared_ptr<TextureColor> albedo) : Material(std::move(albedo)) {}
