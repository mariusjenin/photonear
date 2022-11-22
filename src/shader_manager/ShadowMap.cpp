//
// Created by mariusjenin on 21/11/22.
//

#include "ShadowMap.h"
#include "ShadowMapShaders.h"

using namespace shader_manager;

ShadowMap::ShadowMap(int w, int h, GLuint id_texture_unit_depth_map) {
    {
        m_id_texture_unit_depth_map = id_texture_unit_depth_map;

        //Generating and binding the Frame Buffer
        glGenFramebuffers(1, &m_depth_map_fbo);

        m_width = w;
        m_height = h;

        // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
        glGenTextures(1, &m_depth_map_texture);
        glBindTexture(GL_TEXTURE_2D, m_depth_map_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, m_depth_map_fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_map_texture, 0);

        glDrawBuffer(GL_NONE); // No color buffer is drawn to.

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Shadow Map not successfully created" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

ShadowMap::~ShadowMap() {
    glDeleteFramebuffers(1, &m_depth_map_fbo);
}

void ShadowMap::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_depth_map_fbo);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::unbind_bound_shadow_map() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint ShadowMap::get_depth_map_id_texture() const {
    return m_depth_map_texture;
}

int ShadowMap::get_width() const {
    return m_width;
}

int ShadowMap::get_height() const {
    return m_height;
}

void ShadowMap::print_in_img(const char* path_name) const {
    std::ofstream output_image(path_name);
    // READ THE CONTENT FROM THE FBO
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    auto * pixels = new float [ m_width * m_height ];
    glReadPixels(0, 0, m_width, m_height, GL_DEPTH_COMPONENT , GL_FLOAT, pixels);
    output_image << "P3" << std::endl;
    output_image << m_width << " " << m_height << std::endl;
    output_image << "255" << std::endl;
    int k = 0;
    for(int i=0; i<m_width; i++) {
        for(int j=0; j<m_height; j++) {
            output_image << (unsigned int)(255*pixels[k]) << " " << (unsigned int)(255*pixels[k]) << " " << (unsigned int)(255*pixels[k]) << " ";
            k = k+1;
        }
        output_image << std::endl;
    }
    delete[] pixels;
    output_image.close();
}

void ShadowMap::activate_texture() const {
    glActiveTexture(GL_TEXTURE0 + m_id_texture_unit_depth_map);
    glBindTexture(GL_TEXTURE_2D, m_depth_map_texture);
}
