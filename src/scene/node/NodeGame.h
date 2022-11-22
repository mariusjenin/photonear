//
// Created by mariusjenin on 19/11/22.
//

#ifndef GAME_ENGINE_NODEGAMESG_H
#define GAME_ENGINE_NODEGAMESG_H


#include "Node.h"
#include <src/material/Material.hpp>
#include <src/light/Light.hpp>
#include <src/physics/bounding_box/BoundingBox.hpp>
#include <src/physics/bounding_box/AABB.hpp>
#include "src/mesh/Mesh.hpp"
#include <src/physics/RigidBodyVolume.hpp>

using namespace mesh;
using namespace material;
namespace light{
    class Light;
}
using namespace light;
namespace scene_graph {
    ///NodeSG that can have \link mesh::Mesh Meshes\endlink, light and camera
    class NodeGameSG : public NodeSG {

#define NODE_INIT_POSITION glm::vec3(0, 0, 0)
#define NODE_INIT_FORWARD glm::vec3(0, 0, -1)
#define NODE_INIT_UP glm::vec3(0, 1, 0)

    private:
        //Meshes
        std::vector<Mesh *> m_meshes;
        bool m_meshes_dirty;
        //Drawable
        bool m_drawable;
        //Material
        Material *m_material{};
        bool m_has_material;
        //Light
        Light *m_light;
        //BoundingBox
        BoundingBox *m_bb{};
        BB_TYPE m_bb_type;
        //Render
        bool m_see_both_face;
        //RigidBody
        RigidBodyVolume *m_rigid_body{};
        //Debug Rendering
        bool m_debug_rendering{};
        glm::vec3 m_color_rendering;

        /**
         * Refresh the BoundingBox of the NodeGameSG without applying any Transform
         * @param pos_camera
         * @param force_compute
         * @param change_dirty_flags
         * @return has refresh
         */
        bool refresh_bb_aux(glm::vec3 pos_camera, bool force_compute = false, bool change_dirty_flags = true);

        /**
         * Refresh the BoundingBox of the NodeGameSG applying only the NodeGameSG Transform
         * @param pos_camera
         * @param change_dirty_flags
         * @return has refresh
         */
        bool refresh_bb_recursive(glm::vec3 pos_camera, bool change_dirty_flags = true);

    public:
        /**
         * Constructor of the NodeGameSG
         * @param parent
         * @param bb_type
         */
        NodeGameSG(ElementSG *parent, BB_TYPE bb_type = AABB_TYPE);

        /**
         * Setter of the Debug rendering
         * @param dr
         * @param color_rendering
         */
        void set_debug_rendering(bool dr, glm::vec3 color_rendering = {1, 0, 0});

        bool is_node_game() override;

        /**
         * Getter of the RigidBodyVolume
         * @return rigid_body
         */
        RigidBodyVolume *get_rigid_body() const;

        /**
         * Setter of the RigidBodyVolume
         * @param rigid_body
         */
        void set_rigid_body(RigidBodyVolume *rigid_body);

        /**
         * Refresh the BoundingBox of the NodeGameSG
         * @param pos_camera
         * @param change_dirty_flags
         * @return has refresh
         */
        bool refresh_bb(glm::vec3 pos_camera, bool change_dirty_flags = true);

        /**
         * Set a light at this NodeGameSG
         * @param light
         */
        void set_light(Light *light);

        /**
         * Remove the light to this NodeGameSG
         */
        void remove_light();

        /**
         * Getter of if the NodeGameSG has a light or no
         * @return has light
         */
        bool has_light();

        /**
         * Give the \link light::LightInfo LightInfo\endlink that correspond to the \link light::Light Light\endlink
         * @return light_shader
         */
        LightInfo generate_light_struct();

        /**
         * Update the view matrix in the shader_manager
         * @param shaders
         */
        void update_view_mat(Shaders* shaders);

        /**
         * pdate the view position in the shader_manager
         * @param shaders
         */
        void update_view_pos(Shaders* shaders);

        void draw(Shaders *shaders, glm::vec3 pos_camera, bool allow_debug) override;

        /**
        * Setter of the \link mesh::Mesh Mesh\endlink list of the NodeGameSG
        * @param meshes
        */
        void set_meshes(std::vector<Mesh *> meshes);

        /**
         * Setter of the material applied to the NodeGameSG
         * @param material
         */
        void set_material(Material *material);

        /**
         * Getter of the material
         * @return material
         */
        Material *get_material() const;

        /**
         * Getter of the \link mesh::Mesh Mesh\endlink list of the NodeGameSG
         * @return meshes
         */
        const std::vector<Mesh *> &get_meshes() const;

        /**
         * Getter of whether or not the ElementSG has children
         * @return has meshes
         */
        bool has_meshes() const;

        /**
        * Gives the center of the NodeGameSG (mean of each \link mesh::Mesh Mesh\endlink centers)
        * @param pos_camera
        * @return center
        */
        glm::vec3 get_center(glm::vec3 pos_camera);

        /**
         * Setter of the flag see_both_face (resolve both face of the \link mesh::Mesh Meshes\endlink))
         * @param see_both_face
         */
        void set_see_both_face(bool see_both_face);

        /**
         * Gives the aligned axis bounding box of the NodeGameSG (bounding box of \link mesh::Mesh Mesh\endlink bounding boxes)
         * @param pos_camera
         * @return bounding_box
         */
        std::pair<glm::vec3, glm::vec3> get_aabb(glm::vec3 pos_camera);


        /**
         * Compute the distance between the bounding box and a given position
         * @param cam_position
         * @param position
         * @return distance
         */
        float get_distance_from(glm::vec3 cam_position, glm::vec3 position);

        /**
         * Getter of the BoundingBox
         * @return bb
         */
        BoundingBox *get_bb();

        /**
         * Setter of whether or not the NodeGameSG is drable
         * @param drawable
         */
        void set_drawable(bool drawable);
    };
}

#endif //GAME_ENGINE_NODEGAMESG_H
