//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SHADERS_HPP
#define PHOTONEAR_SHADERS_HPP


#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "GL/glew.h"
#include "glm/glm.hpp"

namespace shader_manager {
    class TextureManager;
    class ShadersDataManager;
    /**
     * Vec3 used in shader
     */
    struct alignas(4) glsl_bool {
        int value;

        glsl_bool();

        /**
         * Construct a glsl_bool from a boolean
         * @param boolean
         */
        explicit glsl_bool(bool boolean);
    };

    /**
     * Vec3 used in shader
     */
    struct alignas(16) glsl_vec3 {
        float x, y, z;

        glsl_vec3();

        /**
         * Construct a glsl_vec3 from a glm::vec3
         * @param v
         */
        explicit glsl_vec3(glm::vec3 v);
    };

    /**
     * Mat4 used in shader
     */
    struct glsl_mat4 {
        float x0, x1, x2, x3, y0, y1, y2, y3, z0, z1, z2, z3, w0, w1, w2, w3;

        glsl_mat4();

        /**
         * Construct a glsl_mat4 from a glm::mat4
         * @param m
         */
        explicit glsl_mat4(glm::mat4 m);
    };

    /// Group the VAOManager, the ShadersDataManager and the TextureManager
    class Shaders {
    protected:
        GLuint m_program_id;
        ShadersDataManager *m_shader_data_manager;
        TextureManager *m_texture_manager;
        bool m_testing_normal;

        /**
         * Constructor of the Shaders given paths to the shaders files
         * @param vertex_file_path
         * @param fragment_file_path
         */
        Shaders(const char *vertex_file_path, const char *fragment_file_path, bool testing_normal = true);
    public:

        bool is_testing_normal() const;

        /**
         * Getter of the program id
         * @return program_id
         */
        GLuint get_program_id() const;

        /**
         * Use the program
         */
        void use() const;

        /**
         * Getter of the ShadersDataManager
         * @return shader_data_manager
         */
        ShadersDataManager *get_shader_data_manager();

        /**
         * Getter of the TextureManager
         * @return texture_manager
         */
        TextureManager *get_texture_manager();

        /// Load all the locations thanks to the ShadersDataManager
        virtual void load_location() const = 0;

    };
}

#endif //PHOTONEAR_SHADERS_HPP
