//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SHADOWMAP_H
#define PHOTONEAR_SHADOWMAP_H

#include "Shaders.h"

namespace shader_manager{
    ///Represents a Shadow Map
    class ShadowMap{
    private:
        GLuint m_depth_map_fbo{};
        GLuint m_depth_map_texture{};
        GLuint m_id_texture_unit_depth_map{};
        int m_width;
        int m_height;
    public:
        /**
         * Constructor of a ShadowMap with size and id_texture
         * @param w
         * @param h
         * @param id_texture_unit_depth_map
         */
        ShadowMap(int w, int h, GLuint id_texture_unit_depth_map);

        /**
         * Destructor of a ShadowMap
         */
        ~ShadowMap();

        /**
         * Bind the ShadowMap for writing
         */
        void bind() const;

        /**
         * Activate the texture
         */
        void activate_texture() const;

        /**
         * Unbind the bound ShadowMap
         */
        static void unbind_bound_shadow_map();

        /**
         * Getter of the id texture of the depth map
         * @return id texture
         */
        GLuint get_depth_map_id_texture() const;

        /**
         * Getter of the width
         * @return width
         */
        int get_width() const;

        /**
         * Getter of the height
         * @return height
         */
        int get_height() const;

        /**
         * Print the ShadowMap in an img
         * @param name
         */
        void print_in_img(const char* path_name) const;
    };
}

#endif //PHOTONEAR_SHADOWMAP_H
