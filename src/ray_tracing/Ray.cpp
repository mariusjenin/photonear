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

Ray::Ray(point origin, versor direction, float time) : m_origin(origin), m_direction(direction){
}
