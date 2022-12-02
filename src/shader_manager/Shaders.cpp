//
// Created by mariusjenin on 21/11/22.
//

#include "Shaders.h"
#include "TextureManager.h"
#include <ShadersDataManager.h>

using namespace shader_manager;

glsl_vec3::glsl_vec3(glm::vec3 v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

glsl_vec3::glsl_vec3() {
    x = 0;
    y = 0;
    z = 0;
}

glsl_bool::glsl_bool() {
    value = 0;
}

glsl_bool::glsl_bool(bool boolean) {
    value = boolean?1:0;
}



glsl_mat4::glsl_mat4(glm::mat4 m) {
    x0 = m[0][0]; x1 = m[0][1];  x2 = m[0][2]; x3 = m[0][3];
    y0 = m[1][0]; y1 = m[1][1];  y2 = m[1][2]; y3 = m[1][3];
    z0 = m[2][0]; z1 = m[2][1];  z2 = m[2][2]; z3 = m[2][3];
    w0 = m[3][0]; w1 = m[3][1];  w2 = m[3][2]; w3 = m[3][3];
}

glsl_mat4::glsl_mat4() {
    x0 = 0; x1 = 0;  x2 = 0; x3 = 0;
    y0 = 0; y1 = 0;  y2 = 0; y3 = 0;
    z0 = 0; z1 = 0;  z2 = 0; z3 = 0;
    w0 = 0; w1 = 0;  w2 = 0; w3 = 0;
}

Shaders::Shaders(const char *vertex_file_path, const char *fragment_file_path, bool testing_normal) {
    m_shader_data_manager = new ShadersDataManager();
    m_texture_manager = new TextureManager(m_shader_data_manager);
    m_testing_normal = testing_normal;

    // Create the shaders
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertex_shader_code;
    std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);
    bool stop_creating_shader = false;
    if (vertex_shader_stream.is_open()) {
        std::stringstream sstr;
        sstr << vertex_shader_stream.rdbuf();
        vertex_shader_code = sstr.str();
        vertex_shader_stream.close();
    } else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n",
               vertex_file_path);
        getchar();
        stop_creating_shader = true;
    }

    if (!stop_creating_shader) {
        // Read the Fragment Shader code from the file
        std::string fragment_shader_code;
        std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);
        if (fragment_shader_stream.is_open()) {
            std::stringstream sstr;
            sstr << fragment_shader_stream.rdbuf();
            fragment_shader_code = sstr.str();
            fragment_shader_stream.close();
        }

        GLint result = GL_FALSE;
        int info_log_length;


        // Compile Vertex Shader
        char const *vertex_source_pointer = vertex_shader_code.c_str();
        glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, nullptr);
        glCompileShader(vertex_shader_id);

        // Check Vertex Shader
        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0) {
            std::vector<char> vertex_shader_error_message(info_log_length + 1);
            glGetShaderInfoLog(vertex_shader_id, info_log_length, nullptr, &vertex_shader_error_message[0]);
            printf("%s\n", &vertex_shader_error_message[0]);
        }



        // Compile Fragment Shader
        char const *fragment_source_pointer = fragment_shader_code.c_str();
        glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, nullptr);
        glCompileShader(fragment_shader_id);

        // Check Fragment Shader
        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0) {
            std::vector<char> fragment_shader_error_message(info_log_length + 1);
            glGetShaderInfoLog(fragment_shader_id, info_log_length, nullptr, &fragment_shader_error_message[0]);
            printf("%s\n", &fragment_shader_error_message[0]);
        }


        // Link the program
        m_program_id = glCreateProgram();
        glAttachShader(m_program_id, vertex_shader_id);
        glAttachShader(m_program_id, fragment_shader_id);
        glLinkProgram(m_program_id);

        // Check the program
        glGetProgramiv(m_program_id, GL_LINK_STATUS, &result);
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0) {
            std::vector<char> program_error_message(info_log_length + 1);
            glGetProgramInfoLog(m_program_id, info_log_length, nullptr, &program_error_message[0]);
            printf("%s\n", &program_error_message[0]);
        }


        glDetachShader(m_program_id, vertex_shader_id);
        glDetachShader(m_program_id, fragment_shader_id);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
    }
}

GLuint Shaders::get_program_id() const {
    return m_program_id;
}

ShadersDataManager *Shaders::get_shader_data_manager() {
    return m_shader_data_manager;
}

TextureManager *Shaders::get_texture_manager() {
    return m_texture_manager;
}

void Shaders::use() const {
    glUseProgram(m_program_id);
}

bool Shaders::is_testing_normal() const {
    return m_testing_normal;
}
