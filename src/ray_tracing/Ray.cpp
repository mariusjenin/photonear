//
// Created by mariusjenin on 19/11/22.
//

#include "Ray.h"

using namespace ray_tracing;

point Ray::get_origin() const {
    return m_origin;
}

versor Ray::get_direction() const{
    return m_direction;
}

Ray::Ray(point origin, versor direction, float t_min, float t_max){
    m_origin = origin;
    m_direction = direction;
    m_t_min = t_min;
    m_t_max = t_max;
}

float Ray::get_t_min() const {
    return m_t_min;
}

float Ray::get_t_max() const {
    return m_t_max;
}
