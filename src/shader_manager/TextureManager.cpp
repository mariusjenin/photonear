//
// Created by mariusjenin on 20/11/22.
//

#include <iostream>
#include "TextureManager.h"
#include "ShadersDataManager.h"

using namespace shader_manager;

TextureManager::TextureManager(ShadersDataManager *shader_data_manager) {
    m_shader_data_manager = shader_data_manager;
    m_next_id_texture = 0;
}

GLuint TextureManager::load_uniform_texture(GLuint program_id, const std::string &name, const std::string &path) {
    m_shader_data_manager->load_custom_uniform_location(program_id, name);
    GLint id_texture = load_texture(path);;
    glUniform1i(m_shader_data_manager->get_location(name), id_texture);
    return id_texture;
}

GLint TextureManager::load_texture(const std::string &path) {
    GLint id_texture = get_and_increment_id_texture();
    load_bmp_custom(path, id_texture);
    return id_texture;
}

GLint TextureManager::get_and_increment_id_texture() {
    GLint id_texture = m_next_id_texture;
    m_next_id_texture++;
    return id_texture;
}

GLuint TextureManager::load_bmp_custom(const std::string &path, int id_texture) {
    char *imagepath = const_cast<char *>(path.c_str());
    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char *data;

    // Open the file
    FILE *file = fopen(imagepath, "rb");
    if (!file) {
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
        getchar();
        return 0;
    }

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file A\n");
        fclose(file);
        return 0;
    }
    // A BMP files always begins with "BM"
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file B\n");
        fclose(file);
        return 0;
    }
    // Make sure this is a 24bpp file
    if (*(int *) &(header[0x1E]) != 0) {
        printf("Not a correct BMP file C\n");
        fclose(file);
        return 0;
    }
    if (*(int *) &(header[0x1C]) != 24) {
        printf("Not a correct BMP file D\n");
        fclose(file);
        return 0;
    }

    // Read the information about the image
    dataPos = *(int *) &(header[0x0A]);
    imageSize = *(int *) &(header[0x22]);
    width = *(int *) &(header[0x12]);
    height = *(int *) &(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0) dataPos = 54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    // Everything is in memory now, the file can be closed.
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    glActiveTexture(GL_TEXTURE0 + id_texture); // activate the texture unit first before binding texture

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL has now copied the data. Free our own version
    delete[] data;

    // Poor filtering, or ...
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // ... nice trilinear filtering ...
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // ... which requires mipmaps. Generate them automatically.
    glGenerateMipmap(GL_TEXTURE_2D);

    // Return the ID of the texture we just created
    return textureID;
}