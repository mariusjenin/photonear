//
// Created by mariusjenin on 29/11/22.
//

#ifndef PHOTONEAR_RAYTRACER_H
#define PHOTONEAR_RAYTRACER_H


#include <vector>
#include <GL/glew.h>
#include "PhotonHit.h"

namespace ray_tracing{
    class RayTracer {
    private:
        int m_sample_by_pixel;
        bool m_auto_size;
        int m_width;
        int m_height;
        bool m_auto_recompute{};
        GLuint m_image_texture{};
        std::vector<unsigned char> m_data;
        std::vector<std::vector<PhotonHit>> m_photon_hit;

        bool m_image_computed{};
        bool m_raytracing_computed{};

        void compute_raytracing();

        void compute_image();

        void reinit_data();
    public:
        RayTracer();

        void update();

        void generate_ui_ray_tracing_settings();

        void generate_ui_viewer();

        void set_raytracing_computed(bool computed);
    };
}


#endif //PHOTONEAR_RAYTRACER_H