//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_BOUNDINGBOX_H
#define PHOTONEAR_BOUNDINGBOX_H

#include "MaskedComponent.h"
#include "Ray.h"

using namespace ray_tracing;

namespace component{
    class BoundingBox : public MaskedComponent{
    private:
        point m_max{};
        point m_min{};
    public:
        BoundingBox();

        void init();

        void merge(BoundingBox bb);

        void merge(const std::vector<point>& vertices);

        ComponentType get_type() override;

        void generate_ui_component_editor() override;

        void draw(const std::shared_ptr<Shaders> &shaders, color color) override;

        std::vector<point> to_vertices();

        bool hit_by_ray(Ray ray);
    };
}


#endif //PHOTONEAR_BOUNDINGBOX_H
