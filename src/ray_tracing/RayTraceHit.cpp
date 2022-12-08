//
// Created by mariusjenin on 07/12/22.
//

#include "RayTraceHit.h"
#include "Shape.h"

using namespace component::shape;
using namespace ray_tracing;

RayTraceHit::RayTraceHit() {
    hit = false;
    t = -FLT_MAX;
    shape = nullptr;
}

void RayTraceHit::merge(RayTraceHit ray_hit) {
    hit = ray_hit.hit;
    t = ray_hit.t;
    u = ray_hit.u;
    v = ray_hit.v;
    normal = ray_hit.normal;
    hit_point = ray_hit.hit_point;
    shape = ray_hit.shape;
}
