//
// Created by mariusjenin on 21/11/22.
//

#include "ShadersBufferManager.h"

using namespace shader_manager;

void ShadersBufferManager::generate_vao(GLuint *vao_id) {
    glGenVertexArrays(1, vao_id);
}

void ShadersBufferManager::generate_bo(GLuint *bo_id) {
    glGenBuffers(1, bo_id);
}

void ShadersBufferManager::bind_vao(GLuint vao_id) {
    glBindVertexArray(vao_id);
}

void ShadersBufferManager::enable_attrib_vbo(GLuint index_vbo, GLuint vbo_id, GLint size_data, GLboolean normalized) {
    glEnableVertexAttribArray(index_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glVertexAttribPointer(index_vbo, size_data, GL_FLOAT, normalized, 0, nullptr);
}

void ShadersBufferManager::delete_bo(GLuint bo_id) {
    glDeleteBuffers(1, &bo_id);
}

void ShadersBufferManager::delete_vao(GLuint vao_id) {
    glDeleteVertexArrays(1, &vao_id);
}

template<typename T>
void ShadersBufferManager::fill_bo(GLenum buffer_type, GLuint vbo_id, std::vector<T> datas) {
    glBindBuffer(buffer_type, vbo_id);
    glBufferData(buffer_type, (int) (datas.size() * sizeof(T)), &datas[0],
                 GL_STATIC_DRAW);
}

// Force instantiation of the template function fill_bo
template void
ShadersBufferManager::fill_bo<unsigned short int>(GLenum buffer_type, GLuint vbo_id, std::vector<unsigned short int> datas);

template void ShadersBufferManager::fill_bo<glm::vec2>(GLenum buffer_type, GLuint vbo_id, std::vector<glm::vec2> datas);

template void ShadersBufferManager::fill_bo<glm::vec3>(GLenum buffer_type, GLuint vbo_id, std::vector<glm::vec3> datas);

void ShadersBufferManager::disable_attrib_vbo(GLuint index_vbo) {
    glDisableVertexAttribArray(index_vbo);
}

void ShadersBufferManager::draw(GLuint ebo_id, long nb_indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glDrawElements(GL_TRIANGLES, (GLsizei) nb_indices, GL_UNSIGNED_SHORT, nullptr);
}

void ShadersBufferManager::draw_verticies_debug(GLenum type, std::vector<glm::vec3> verticies) {
    size_t verticies_size = verticies.size();
    if((int)verticies_size>0){

        glLineWidth(100.f);
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vertex_buffer;
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, (long)verticies_size * (long)sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0, nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glDrawArrays(type, 0, (int)verticies_size);

        glDeleteBuffers(1, &vertex_buffer);
        glDeleteVertexArrays(1, &vao);
    }
}

ShadersBufferManager::ShadersBufferManager() = default;

