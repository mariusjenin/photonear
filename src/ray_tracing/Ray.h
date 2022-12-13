//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_RAY_H
#define PHOTONEAR_RAY_H

#include "Transform.h"

namespace ray_tracing {
    class Ray {
    private:
        point m_origin{};
        versor m_direction{};
        float m_t_min{};
        float m_t_max{};
    public:
        Ray() = default;

        Ray(point origin, versor direction, float t_min = 0, float t_max = 10000);

        point get_origin() const;

        versor get_direction() const;

        float get_t_min()const;

        float get_t_max()const;
    };
}


#endif //PHOTONEAR_RAY_H
