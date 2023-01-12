//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_PHOTON_H
#define PHOTONEAR_PHOTON_H

#include "glm/glm.hpp"
#include "vec3_type.h"
#include "RayCastHit.h"

namespace ray_tracing{
    class RayCastHit;
}

namespace ray_tracing{
    struct Photon {
        point position{};
        versor direction{}; //away from surface
        versor normal{};
        Shape* shape{};
        color color_photon{};
        float weight;

        explicit Photon(const std::shared_ptr<RayCastHit>& ray_hit);
    };
}


#endif //PHOTONEAR_PHOTON_H
