//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_SHAPE_H
#define PHOTONEAR_SHAPE_H

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "vector"
#include "Component.h"
#include "Shaders.h"
#include "BoundingBox.h"
#include "RayCastHit.h"

using namespace shader_manager;
using namespace ray_tracing;

namespace component {
    namespace shape {
        /// Shape shape by a ray
        class Shape : public Component {
        protected:
            // Shader
            bool m_loaded_vao{};
            GLuint m_vbo_position_id{}, m_vbo_tex_coords_id{}, m_vbo_normals_id{}, m_ebo_triangle_indices_id{};
            GLuint m_vao_id{};

            std::vector<point> m_vertex_positions;
            std::vector<unsigned short int> m_triangle_indices;
            std::vector<vec2> m_vertex_tex_coords;
            std::vector<versor> m_vertex_normals;

            bool m_both_face_visible;

            explicit Shape(bool both_face_visible = true);

        public:
            void load_mesh_in_vao();

            ComponentType get_type() override;

            void draw(const std::shared_ptr<Shaders> &shaders);

            void generate_ui_component_editor() override;

            virtual std::vector<point> to_few_vertices() =0;

            virtual RayCastHit hit(Ray ray)=0;

            ~Shape();
        };
    }
}

#endif //PHOTONEAR_SHAPE_H
