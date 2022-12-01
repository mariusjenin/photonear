//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SHADERSBUFFERMANAGER_HPP
#define PHOTONEAR_SHADERSBUFFERMANAGER_HPP

#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

namespace shader_manager {
    /// Manager of VBOs and EBO
    class ShadersBufferManager {
    private:
        /**
         * Constructor of ShadersBufferManager
         */
        ShadersBufferManager();

    public:

        const static int ID_VERTEX_BUFFER = 0;
        const static int ID_NORMAL_BUFFER = 1;
        const static int ID_UV_BUFFER = 2;

        /**
         * Generate a new VAO
         * @param vao_id
         */
        static void generate_vao(GLuint *vao_id);

        /**
         * Generate a new VBO
         * @param bo_id
         */
        static void generate_bo(GLuint *bo_id);

        /**
         * Bind the VAO with the given id
         * @param vao_id
         */
        static void bind_vao(GLuint vao_id);

        /**
         * Enable the pointer for a VBO
         * @param index_vbo
         * @param vbo_id
         * @param size_data
         * @param normalized
         */
        static void enable_attrib_vbo(GLuint index_vbo, GLuint vbo_id, GLint size_data, GLboolean normalized);

        /**
         * Delete a BO
         * @param bo_id
         */
        static void delete_bo(GLuint bo_id);

        /**
         * Delete a VAO
         * @param vao_id
         */
        static void delete_vao(GLuint vao_id);

        /**
         * Fill a buffer with datas
         * @tparam T
         * @param buffer_type
         * @param vbo_id
         * @param datas
         */
        template<typename T>
        static void fill_bo(GLenum buffer_type, GLuint vbo_id, std::vector<T> datas);

        /**
         * Disable the pointer for a VBO
         * @param index_vbo
         */
        static void disable_attrib_vbo(GLuint index_vbo);

        /**
         * Draw the EBO with the given id
         * @param ebo_id
         * @param nb_indices
         */
        static void draw(GLuint ebo_id, long nb_indices);

        /**
         * Draw a list of verticies as a debug rendering
         * @param type
         * @param verticies
         */
        static void draw_verticies_debug(GLenum type, std::vector<glm::vec3> verticies);

    };
}

#endif //PHOTONEAR_SHADERSBUFFERMANAGER_HPP
