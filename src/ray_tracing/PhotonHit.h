//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_PHOTONHIT_H
#define PHOTONEAR_PHOTONHIT_H

#include "glm/glm.hpp"

namespace ray_tracing{
    class PhotonHit {
    private:
        glm::vec3 m_position;
        glm::vec3 m_normal;
        glm::vec3 m_ray_direction;
        glm::vec2 m_pixel_position;
        float m_radius;
        int m_n_photon;
        glm::vec3 m_color;
    };
}


#endif //PHOTONEAR_PHOTONHIT_H
