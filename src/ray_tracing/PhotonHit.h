//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_PHOTONHIT_H
#define PHOTONEAR_PHOTONHIT_H

#include "glm/glm.hpp"
#include "Transform.h"

namespace ray_tracing{
    class PhotonHit {
    private:
        point m_position;
        versor m_normal;
        versor m_ray_direction;
        point m_pixel_position;
        float m_radius;
        int m_n_photon;
        color m_color;
    };
}


#endif //PHOTONEAR_PHOTONHIT_H
