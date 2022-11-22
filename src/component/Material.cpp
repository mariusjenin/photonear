//
// Created by mariusjenin on 19/11/22.
//

#include "Material.h"

using namespace component;

std::shared_ptr<Material> Material::get_default() {
    return std::make_shared<Material>();
}

ComponentType Material::get_type() {
    return typeid(this);
}

