//
// Created by mariusjenin on 21/11/22.
//

#ifndef PHOTONEAR_SPHERE_H
#define PHOTONEAR_SPHERE_H

#include "glm/glm.hpp"
#include "Hittable.h"

using namespace std;
using namespace glm;

namespace component {
    class Sphere : public Hittable {
    private:
        glm::vec3 m_center;
        float m_radius;

        static glm::vec3 spheric_to_euclidian_coords(float theta, float phi) {
            return {cos(theta) * cos(phi), sin(theta) * cos(phi), sin(phi)};
        }

        void assign_mesh_sphere(float radius, int slices, int stacks);

//    // Mesh Data :
//    // std::vector<glm::vec3> vertex_positions;
//    //  std::vector<unsigned short int> triangle_indices;
//    //  std::vector<glm::vec2> vertex_tex_coords;
//    //  std::vector<glm::vec3> vertex_normals;
//
//    MeshData create_sphere(
//            float radius, int slices, int stacks) {
//        std::vector<vec3> vertices = std::vector<vec3>();
//        std::vector<vec3> normals = std::vector<vec3>();
//        std::vector<unsigned short int> indices = std::vector<unsigned short int>();
//        std::vector<vec2> uv = std::vector<vec2>();
//
//        float du = 2 * M_PI / slices;
//        float dv = M_PI / stacks;
//        int i, j;
//        float u, v, x, y, z;
//        for (i = 0; i <= stacks; i++) {
//            v = (float) -M_PI / 2 + (float) i * dv;
//            for (j = 0; j <= slices; j++) {
//                u = j * du;
//                x = cos(u) * cos(v);
//                y = sin(u) * cos(v);
//                z = sin(v);
//                vertices.emplace_back(radius * x, radius * y, radius * z);
//                normals.emplace_back(radius * x, radius * y, radius * z);
//                uv.emplace_back((float) j / (float) slices, (float) i / (float) stacks);
//            }
//        }
//        for (j = 0; j < stacks; j++) {
//            int row1 = j * (slices + 1);
//            int row2 = (j + 1) * (slices + 1);
//            for (i = 0; i < slices; i++) {
//                indices.push_back(row1 + i);
//                indices.push_back(row2 + i + 1);
//                indices.push_back(row2 + i);
//                indices.push_back(row1 + i);
//                indices.push_back(row1 + i + 1);
//                indices.push_back(row2 + i + 1);
//            }
//        }
//        return {vertices, indices, uv, normals};
//    }
    public:
        Sphere(glm::vec3 center = {0,0,0}, float radius = 1, int slices = 20, int stacks = 20);
    };
}

#endif //PHOTONEAR_SPHERE_H
