//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_BOUNDINGBOX_H
#define PHOTONEAR_BOUNDINGBOX_H

#include "MaskedComponent.h"

namespace component{
    class BoundingBox : public MaskedComponent{
    private:
        glm::vec3 m_max{};
        glm::vec3 m_min{};
    public:
        BoundingBox();

        void init();

        void merge(BoundingBox bb);

        void merge(const std::vector<glm::vec3>& vertices);

        void merge(glm::vec3 min , glm::vec3 max);

        ComponentType get_type() override;

        void generate_ui_component_editor() override;

        void draw(const std::shared_ptr<Shaders> &shaders) override;

        std::vector<glm::vec3> to_vertices();
    };
}


#endif //PHOTONEAR_BOUNDINGBOX_H
