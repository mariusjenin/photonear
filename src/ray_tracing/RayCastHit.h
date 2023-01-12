//
// Created by mariusjenin on 07/12/22.
//

#ifndef PHOTONEAR_RAYCASTHIT_H
#define PHOTONEAR_RAYCASTHIT_H

#include <vector>
#include "memory"
#include "vec3_type.h"
#include "Shape.h"

namespace component{
    namespace shape{
        class Shape;
    }
}

using namespace component::shape;

namespace ray_tracing {
    struct RayCastHit {
        // Initialized on shape hit
        bool hit;
        float t;
        point hit_point{};
        versor normal{};
        versor direction{};
        float u{},v{};
        bool inner_shape;
        Shape* shape;

        //Initialized on creation
        float weight;

        //Initialized on resolve ray or initilization of the first rays
        bool contribute;
        color attenuation{};
        float refractive_index_of_medium{};

        std::vector<std::shared_ptr<RayCastHit>> bounce_rays;

        RayCastHit();

        void merge(const RayCastHit& ray_hit);
    };
}

#endif //PHOTONEAR_RAYCASTHIT_H
