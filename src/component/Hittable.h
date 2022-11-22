//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_HITTABLE_H
#define PHOTONEAR_HITTABLE_H

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "vector"
#include "Component.h"
#include "ShadersBufferManager.h"


namespace component {
    struct MeshData{
    public:
        std::vector<glm::vec3> vertex_positions;
        std::vector<unsigned short int> triangle_indices;
        std::vector<glm::vec2> vertex_tex_coords;
        std::vector<glm::vec3> vertex_normals;
    };

    /// Shape hittable by a ray
    class Hittable : public Component {
    protected:
        bool m_up_to_date;

        // Shader
        bool m_loaded_vao;
        GLuint m_vbo_position_id{}, m_vbo_tex_coords_id{}, m_vbo_normals_id{}, m_ebo_triangle_indices_id{};
        GLuint m_vao_id{};
        std::vector<glm::vec3> m_vertex_positions;
        std::vector<unsigned short int> m_triangle_indices;
        std::vector<glm::vec2> m_vertex_tex_coords;
        std::vector<glm::vec3> m_vertex_normals;

        Hittable();
    public:
        void load_mesh_in_vao();

        ComponentType get_type() override;

        GLuint get_vao_id() const;

        GLuint get_ebo_triangle_indices_id() const;

        std::vector<unsigned short int> get_triangle_indices();

        ~Hittable();
    };
}

#endif //PHOTONEAR_HITTABLE_H
