//
// Created by mariusjenin on 07/12/22.
//

#ifndef PHOTONEAR_RAYTRACEHIT_H
#define PHOTONEAR_RAYTRACEHIT_H

#include "vec3_type.h"

namespace component{
    namespace shape{
        class Shape;
    }
}

using namespace component::shape;

namespace ray_tracing {
    struct RayTraceHit {
        bool hit;
        float t;
        float u{},v{};
        versor normal{};
        point hit_point{};
        Shape* shape;

        RayTraceHit();

        void merge(RayTraceHit ray_hit);
    };
}

#endif //PHOTONEAR_RAYTRACEHIT_H
