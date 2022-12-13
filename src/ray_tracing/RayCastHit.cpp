//
// Created by mariusjenin on 07/12/22.
//

#include "RayCastHit.h"
#include "Shape.h"

using namespace component::shape;
using namespace ray_tracing;

RayCastHit::RayCastHit() {
    hit = false;
    inner_shape = false;
    t = -FLT_MAX;
    weight = 1;
    diffuse = false;
    shape = nullptr;
    children = {};
    attenuation = {1,1,1};
}

void RayCastHit::merge(const RayCastHit& ray_hit) {
    hit = ray_hit.hit;
    t = ray_hit.t;
    u = ray_hit.u;
    v = ray_hit.v;
    normal = ray_hit.normal;
    hit_point = ray_hit.hit_point;
    shape = ray_hit.shape;
    inner_shape = ray_hit.inner_shape;
}
