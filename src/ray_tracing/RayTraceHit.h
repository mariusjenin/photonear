//
// Created by mariusjenin on 12/12/22.
//

#ifndef PHOTONEAR_RAYTRACEHIT_H
#define PHOTONEAR_RAYTRACEHIT_H

#include "glm/glm.hpp"
#include "vec3_type.h"

namespace ray_tracing {
    struct RayTraceHit {

        int px_u{};
        int px_v{};

        point hit_point{};
        versor normal{};
        versor direction{};
        float weight{};
        color attenuation{};

        color color_in_buffer{};
        float radius;
        int nb_photons;

        RayTraceHit(int u, int v);
    };
}


#endif //PHOTONEAR_RAYTRACEHIT_H
