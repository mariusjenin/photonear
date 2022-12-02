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
    public:
        Ray() = default;

        Ray(point origin, versor direction, float time = 0.0f);

        point get_origin() const;

        versor get_direction() const;
    };
}


#endif //PHOTONEAR_RAY_H
