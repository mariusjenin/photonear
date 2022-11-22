//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SHADOWMAPSHADERS_H
#define PHOTONEAR_SHADOWMAPSHADERS_H

#include "ShadersDataManager.h"
#include "Shaders.h"
//#include "src/light/LightShader.hpp"
#include <iostream>

namespace shader_manager {
    ///Manager of the Shadow Mapping Shaders
    class ShadowMapShaders : public Shaders{
    public:
        /**
         * Constructor of the ShadowMapShaders
         * @param vertex_file_path
         * @param fragment_file_path
         */
        ShadowMapShaders(const char *vertex_file_path, const char *fragment_file_path);

        void load_location() const override;
    };
}


#endif //PHOTONEAR_SHADOWMAPSHADERS_H
