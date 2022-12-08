//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_PHOTON_H
#define PHOTONEAR_PHOTON_H

#include "glm/glm.hpp"
#include "vec3_type.h"

namespace ray_tracing{
    struct Photon {
        point hit_point;
        versor normal;
        versor ray_direction;
        point pixel_position;
        float radius;
        int amount;
        color color_photon;
    };
}


#endif //PHOTONEAR_PHOTON_H
