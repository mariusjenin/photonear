//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_MATERIAL_H
#define PHOTONEAR_MATERIAL_H


#include "Component.h"

namespace component {
    class Material : public Component {
    public:
        static std::shared_ptr<Material> get_default();

        ComponentType get_type() override;
    };
}

#endif //PHOTONEAR_MATERIAL_H
