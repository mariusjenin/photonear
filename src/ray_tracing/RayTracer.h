//
// Created by mariusjenin on 29/11/22.
//

#ifndef PHOTONEAR_RAYTRACER_H
#define PHOTONEAR_RAYTRACER_H


#include <vector>
#include <future>
#include <GL/glew.h>
#include "SceneGraph.h"
#include "PhotonHit.h"

using namespace scene;

namespace ray_tracing{
    class RayTracer {
    private:
        int m_sample_by_pixel;
        bool m_auto_size;
        int m_width{};
        int m_height{};
        bool m_auto_recompute{};
        GLuint m_image_texture{};
        std::vector<unsigned char> m_data;
        std::vector<std::vector<PhotonHit>> m_photon_hit;

        std::future<void> m_async_ray_tracing;
        std::mutex m_image_size_mutex;

        bool m_image_valid{};
        bool m_ray_tracing_valid{};

        bool m_is_computing;
        float m_px_computed;

        void compute_raytracing();

        void compute_raytracing_ray(const std::shared_ptr<SceneGraph>& scene_graph,int x, int y, vec3 origin,vec3 direction,float z_near,float z_far);

        void compute_image();

        void init_ray_tracing_data();
    public:
        RayTracer();

        void update();

        void init();

        void generate_ui_ray_tracing_settings();

        void generate_ui_viewer();

        void generate_ui_logs() const;

        void set_ray_tracing_valid(bool computed);

        bool is_ray_tracing_valid() const;
    };
}


#endif //PHOTONEAR_RAYTRACER_H
