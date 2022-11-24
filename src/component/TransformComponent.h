//
// Created by mariusjenin on 24/11/22.
//

#ifndef PHOTONEAR_TRANSFORMCOMPONENT_H
#define PHOTONEAR_TRANSFORMCOMPONENT_H


#include "Component.h"
#include "Shaders.h"

using namespace shader_manager;

namespace component {
    class TransformComponent : public Component {
    private:
        std::shared_ptr<Transform> m_transform;
        std::shared_ptr<Transform> m_local_transform;
    public:
        TransformComponent();

        TransformComponent(std::shared_ptr<Transform> trsf, std::shared_ptr<Transform> local_trsf);

        std::shared_ptr<Transform> get_transform() const;

        std::shared_ptr<Transform> get_local_transform() const;

        void compute_trsf_scene_graph();

        static void compute_trsf_scene_graph_node(AbstractNode* node);

        glm::mat4 get_matrix(bool inverse = false);

        glm::mat4 get_matrix_as_end_node(bool inverse = false);

        void load_in_shaders(const std::shared_ptr<Shaders>& shaders);

        ComponentType get_type() override;
    };
}

#endif //PHOTONEAR_TRANSFORMCOMPONENT_H
