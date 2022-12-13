//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_PHOTON_H
#define PHOTONEAR_PHOTON_H

#include "glm/glm.hpp"
#include "vec3_type.h"

namespace ray_tracing{
    struct Photon {
        point position;
        versor direction;
        versor normal;
        color color_photon;
        float weight;
    };
}


#endif //PHOTONEAR_PHOTON_H
