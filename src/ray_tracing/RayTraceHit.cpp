//
// Created by mariusjenin on 12/12/22.
//

#include "RayTraceHit.h"

using namespace ray_tracing;

RayTraceHit::RayTraceHit(int u, int v) {
    nb_photons = 0;
    radius = 1;
    color_in_buffer = {0,0,0};
    px_u = u;
    px_v = v;
}
