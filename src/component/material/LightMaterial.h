//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_LIGHTMATERIAL_H
#define PHOTONEAR_LIGHTMATERIAL_H

#include "Material.h"

namespace component{
    namespace material{
        class LightMaterial : public Material {
        protected:
            explicit LightMaterial(std::shared_ptr<TextureColor> albedo = std::make_shared<TextureColor>(1.0f));

        public:
            virtual Light generate_light();
            const static int LIGHT_TYPE_DIRECTIONAL = 0;
            const static int LIGHT_TYPE_POINT = 1;
            const static int LIGHT_TYPE_SPOT = 2;
        };
    }
}


#endif //PHOTONEAR_LIGHTMATERIAL_H
