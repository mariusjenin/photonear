//
// Created by mariusjenin on 19/11/22.
//

#ifndef PHOTONEAR_RAY_H
#define PHOTONEAR_RAY_H

#ifndef RAY_H
#define RAY_H

#include "Transform.h"

class Ray {
private:
    point m_origin{};
    versor m_direction{};
    float m_time{};
public:
    Ray() = default;

    Ray(point origin, versor direction, float time = 0.0f)
            : m_origin(origin), m_direction(direction), m_time(time) {}

    point origin() const { return m_origin; }

    versor direction() const { return m_direction; }

    double time() const { return m_time; }

    point at(float t) const {
        return m_origin + t * m_direction;
    }
};

#endif


#endif //PHOTONEAR_RAY_H