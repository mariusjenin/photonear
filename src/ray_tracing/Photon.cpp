//
// Created by mariusjenin on 30/11/22.
//

#include "Photon.h"
using namespace ray_tracing;

Photon::Photon(const std::shared_ptr<RayCastHit>& ray_hit) {
    position = ray_hit->hit_point;
    direction = -ray_hit->direction;
    normal = ray_hit->normal;
    color_photon = ray_hit->attenuation;
    weight = ray_hit->weight;
    shape = ray_hit->shape;
}
