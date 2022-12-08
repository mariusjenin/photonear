//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SHADERSDATAMANAGER_HPP
#define PHOTONEAR_SHADERSDATAMANAGER_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <vector>
#include "Light.h"

namespace shader_manager {
    class LightShader;
    /// Manager of the locations in shaders
    class ShadersDataManager {
    private:
        std::map<std::string, GLint> m_locations;
    public:
        //MATRICES
        static constexpr const char *MODEL_MAT_LOC_NAME = "model_mat";
        static constexpr const char *VIEW_MAT_LOC_NAME = "view_mat";
        static constexpr const char *PROJ_MAT_LOC_NAME = "projection_mat";
        static constexpr const char *NORMAL_MODEL_MAT_LOC_NAME = "normal_model_mat";
        //CAMERA POSITION
        static constexpr const char *VIEW_POS_LOC_NAME = "view_pos";
        //LIGHTS
        static constexpr const char *NB_LIGTHS_LOC_NAME = "nb_lights";
        static constexpr const char *BLOCK_INDEX_LIGHTS_LOC_NAME = "lights_buffer";
        //MATERIAL TYPE
        static constexpr const char *MATERIAL_TYPE_COLOR_LOC_NAME = "MATERIAL_ALBEDO_TYPE_COLOR";
        static constexpr const char *MATERIAL_TYPE_TEXTURE_LOC_NAME = "MATERIAL_ALBEDO_TYPE_TEXTURE";
        //MATERIAL
        static constexpr const char *MATERIAL_TYPE_LOC_NAME = "material.type_albedo";
        static constexpr const char *MATERIAL_ALBEDO_COLOR_LOC_NAME = "material.albedo";
        static constexpr const char *MATERIAL_ALBEDO_TEXTURE_LOC_NAME = "material.albedo_texture";
        static constexpr const char *MATERIAL_EMISSIVE_LOC_NAME = "material.emissive";
        static constexpr const char *MATERIAL_TRANSPARENCY_LOC_NAME = "material.transparency";
        //LIGHT TYPE
        static constexpr const char *LIGHT_TYPE_DIRECTIONAL_LOC_NAME = "LIGHT_TYPE_DIRECTIONAL";
        static constexpr const char *LIGHT_TYPE_POINT_LOC_NAME = "LIGHT_TYPE_POINT";
        static constexpr const char *LIGHT_TYPE_SPOT_LOC_NAME = "LIGHT_TYPE_SPOT";
        //SHADOW MAP
        static constexpr const char *SHADOW_MAP_ARRAY_LOC_NAME = "shadow_maps";
        static constexpr const char *SHADOW_MAP_DEPTH_VP_MAT_LOC_NAME = "depth_vp_mat";
        //DEBUG RENDERING
        static constexpr const char *DEBUG_RENDERING_LOC_NAME = "debug_rendering";
        static constexpr const char *DEBUG_RENDERING_COLOR_LOC_NAME = "debug_rendering_color";

        /**
         * Load the location of the matrices
         * @param program_id
         */
        void load_matrices_locations(GLuint program_id);

        /**
         * Load the location of the view position
         * @param program_id
         */
        void load_view_pos_location(GLuint program_id);

        /**
         * Load the location and the datas of the \link light::Light Light\endlink Type Constants
         * @param program_id
         */
        void load_lights_const(GLuint program_id);

        /**
         * Load the locations of the \link light::Light Light\endlink system
         * @param program_id
         */
        void load_lights_locations(GLuint program_id);


        /**
         * Load the locations of the debug rendering constant
         * @param program_id
         */
        void load_debug_const(GLuint program_id);

        /**
         * Load the location and the datas of the \link material::Material Material\endlink Type Constants
         * @param program_id
         */
        void load_material_const(GLuint program_id);

        /**
         * Load the locations of the \link material::Material Material\endlink system
         * @param program_id
         */
        void load_material_locations(GLuint program_id);

        /**
         * Load the location for a custom data
         * @param program_id
         * @param name
         */
        void load_custom_uniform_location(GLuint program_id, const std::string &name);

        /**
         * Getter of a data given the name of it
         * @param name
         * @return location
         */
        GLint get_location(const std::string &name);

        /**
         * Load the data of the lights given a list of \link light::LightShader LightShader\endlink
         * @param program_id
         * @param lights_shader
         * @param size_lights
         */
        void load_lights(GLuint program_id,  LightShader lights_shader[], int size_lights);

        /**
         * Load the location of the matrix of the shadow map
         * @param program_id
         */
        void load_shadow_map_matrix_location(GLuint program_id);

        /**
         * Load the locations of the shadow map
         * @param program_id
         */
        void load_shadow_maps_location(GLuint program_id);
    };
}
#endif //PHOTONEAR_SHADERSDATAMANAGER_HPP
