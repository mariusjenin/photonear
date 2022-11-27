//
// Created by mariusjenin on 20/11/22.
//


#ifndef PHOTONEAR_TEXTUREMANAGER_H
#define PHOTONEAR_TEXTUREMANAGER_H

#include <GL/glew.h>
#include "ShadersDataManager.h"
//#include <src/utils/texture.h>

namespace shader_manager {
    ///Manager of all the texture in the shaders
    class TextureManager {
    private:
        GLint m_next_id_texture;
        ShadersDataManager* m_shader_data_manager;
    public:
        /**
         * Constructor of the TextureManager
         * @param shader_data_manager
         */
        explicit TextureManager(ShadersDataManager* shader_data_manager);

        /**
         * Load a new texture in the shader
         * @param program_id
         * @param name
         * @param path
         * @return id_texture
         */
        GLuint load_uniform_texture(GLuint program_id, const std::string &name, const std::string &path);

        /**
         * Load a texture at an existant location in the shader
         * @param path
         * @return id_texture
         */
        GLint load_texture(const std::string &path);

        /**
         * Getter of the next id_texture and increment it
         * @return id_texture
         */
        GLint get_and_increment_id_texture();

        static GLuint load_bmp_custom(const std::string &path, int id_texture);
    };
}

#endif //PHOTONEAR_TEXTUREMANAGER_H
