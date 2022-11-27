//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_VERTFRAGSHADERS_H
#define PHOTONEAR_VERTFRAGSHADERS_H

#include <memory>
#include "Shaders.h"
#include "ShadowMapShaders.h"

namespace shader_manager{
    class VertFragShaders : public Shaders{
    private:
        std::shared_ptr<ShadowMapShaders> m_shadow_map_shaders;
    public:
        /**
         * Constructor of the MainShaders
         * @param vertex_file_path
         * @param fragment_file_path
         */
        VertFragShaders(const char *vertex_file_path, const char *fragment_file_path);

        void load_location() const override;

        /**
         * Getter of the Shaders of the Shadow Map
         * @return shadow_map_shaders
         */
        std::shared_ptr<ShadowMapShaders> get_shadow_map_shaders() const;
    };
}


#endif //PHOTONEAR_VERTFRAGSHADERS_H
