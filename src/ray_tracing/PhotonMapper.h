//
// Created by mariusjenin on 30/11/22.
//

#ifndef PHOTONEAR_PHOTONMAPPER_H
#define PHOTONEAR_PHOTONMAPPER_H

#include <vector>
#include <memory>
#include <future>
#include "Photon.h"
#include "RayCastHit.h"
#include "SceneGraph.h"
#include "PhotonMap.h"
#include "EmissiveMaterial.h"

using namespace scene;
using namespace component::material;

namespace ray_tracing{
    class PhotonMapper {
    private:
        int m_num_pass;
        int m_photon_by_light_by_pass;
        int m_photon_computed;
        int m_nb_total_photons;
        int m_max_depth;
        bool m_pending_ray_tracing;
        bool m_auto_recompute;
        bool m_photon_mapping_valid;

        std::vector<std::shared_ptr<Photon>> m_photon_map_array;
        std::shared_ptr<PhotonMap> m_photon_map;

        std::future<void> m_async_photon_mapping;

        std::mutex m_photon_mapping_mutex;

        void init_photon_map();

        void reinit();
    public:
        PhotonMapper();

        void init();

        void update();

        void generate_ui_photon_mapping_settings();

        void compute_photon_mapping_pass();

        void compute_ray_trace(SceneGraph* scene_graph,color default_color,  Ray ray, EmissiveMaterial* light_material);

        void compute_photon_trace(const std::shared_ptr<RayCastHit>& ray_hit);

        void set_photon_mapping_valid(bool valid);

        void set_pending_ray_tracing(bool pending);

        void generate_ui_logs() const;

        std::shared_ptr<PhotonMap> get_photon_map();

        int get_nb_total_photons() const;

        void reinit_count_pass();

        void lock_photon_mapping();

        void unlock_photon_mapping();
    };
}


#endif //PHOTONEAR_PHOTONMAPPER_H
