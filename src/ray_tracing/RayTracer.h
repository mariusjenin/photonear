//
// Created by mariusjenin on 29/11/22.
//

#ifndef PHOTONEAR_RAYTRACER_H
#define PHOTONEAR_RAYTRACER_H


#include <vector>
#include <future>
#include <GL/glew.h>
#include "SceneGraph.h"
#include "RayTraceHit.h"
#include "Photon.h"
#include "PhotonMap.h"

using namespace scene;

namespace ray_tracing{
    class RayTracer {
    private:
        int m_sample_by_pixel;
        bool m_auto_size;
        int m_width{};
        int m_height{};
        int m_max_depth{};
        color m_default_color{};
        float m_radius_photon_gathering;

        bool m_auto_recompute{};
        GLuint m_image_texture{};
        std::vector<unsigned char> m_image;
        std::vector<std::shared_ptr<RayTraceHit>> m_ray_trace_data;

        std::future<void> m_async_ray_tracing;
        std::mutex m_image_mutex;
        std::mutex m_ray_tracing_mutex;

        bool m_image_valid{};
        bool m_photon_gathering_valid{};
        bool m_ray_tracing_valid{};
        bool m_photon_map_available;

        bool m_is_ray_tracing;
        bool m_is_photon_gathering;

        bool m_first_photon_gathering;

        int m_px_ray_traced;
        int m_ray_photon_gathered;
        int m_total_ray_photon_splatted;

        void compute_ray_tracing_pass();

        void compute_ray_cast(SceneGraph* scene_graph, int u, int v, Ray ray);

        void compute_ray_trace(int u, int v, const std::shared_ptr<RayCastHit>& ray_hit);

        void compute_image();

        void init_ray_tracing_data();

        void on_radius_changed();

        void compute_photon_gathering();

        void photon_gathering(const std::shared_ptr<RayTraceHit>& ray_trace_hit, const std::shared_ptr<PhotonMap>& photon_map, int nb_total_photons);

        void refine_photon_gathering(const std::shared_ptr<RayTraceHit>& ray_trace_hit, const std::shared_ptr<PhotonMap>& photon_map, int nb_total_photons);
    public:
        RayTracer();

        void update();

        void init();

        void generate_ui_ray_tracing_settings();

        void generate_ui_photon_gathering_settings();

        void generate_ui_viewer();

        void generate_ui_ray_tracing_logs() const;

        void generate_ui_photon_gathering_logs() const;

        void set_ray_tracing_valid(bool valid);

        void set_photon_gathering_valid(bool valid);

        void set_photon_map_available(bool available);

        void on_photon_mapping_reinit();

        color get_default_color();
    };
}


#endif //PHOTONEAR_RAYTRACER_H
