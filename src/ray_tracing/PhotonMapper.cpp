//
// Created by mariusjenin on 30/11/22.
//

#include "imgui.h"
#include "PhotonMapper.h"
#include "Photonear.h"
#include "DielectricMaterial.h"

using namespace ray_tracing;

PhotonMapper::PhotonMapper() {
    m_photon_by_light_by_pass = 1000;
    m_nb_total_photons = 0;
    m_photon_computed = 0;
    m_max_depth = 5;
    m_pending_ray_tracing = true;
    m_auto_recompute = false;
    m_photon_mapping_valid = false;
    m_photon_mapping_computed = false;
    m_num_pass = 0;
    m_photon_map = std::make_shared<PhotonMap>();
}

void PhotonMapper::init() {
    init_photon_map();
}

void PhotonMapper::reinit() {
    m_photon_mapping_valid = false;
    m_photon_mapping_computed = false;
    m_nb_total_photons = 0;
    m_photon_computed = 0;
    Photonear::get_instance()->get_ray_tracer()->on_photon_mapping_reinit();
    Photonear::get_instance()->get_ray_tracer()->set_photon_gathering_valid(false);
    Photonear::get_instance()->get_ray_tracer()->set_photon_map_available(false);
    reinit_count_pass();
    init_photon_map();
}

void PhotonMapper::update() {
    if (m_auto_recompute && !m_photon_mapping_valid) {
        if (!m_pending_ray_tracing) {
            Photonear::get_instance()->get_ray_tracer()->set_photon_gathering_valid(false);
            init_photon_map();
        } else {
            reinit();
        }
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
        m_async_photon_mapping = std::async(&PhotonMapper::compute_photon_mapping_pass, this);
#pragma clang diagnostic pop
    }
}

void PhotonMapper::generate_ui_photon_mapping_settings() {

    int photon_by_light_by_pass = m_photon_by_light_by_pass;
    int max_depth = m_max_depth;

    ImGui::Checkbox("Auto compute Photon Mapping", &m_auto_recompute);

    if (ImGui::Button("Recompute First Pass")) {
        if (m_async_photon_mapping.valid()) m_async_photon_mapping.wait();
        reinit();
        m_async_photon_mapping = std::async(&PhotonMapper::compute_photon_mapping_pass, this);
    }

    ImGui::SameLine();

    bool pm_invalid = !m_photon_mapping_valid;
    if (!pm_invalid) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    if (ImGui::Button("Compute a Pass")) {
        if (pm_invalid) {
            if (m_async_photon_mapping.valid()) m_async_photon_mapping.wait();
            Photonear::get_instance()->get_ray_tracer()->set_photon_gathering_valid(false);
            init_photon_map();
            m_async_photon_mapping = std::async(&PhotonMapper::compute_photon_mapping_pass, this);
        }
    }
    if (!pm_invalid) ImGui::PopStyleVar();

    ImGui::SameLine();

    if (pm_invalid) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    if (ImGui::Button("Stop and Reinit")) {
        if (!pm_invalid) {
            if (m_async_photon_mapping.valid()) m_async_photon_mapping.wait();
            reinit();
            Photonear::get_instance()->get_ray_tracer()->set_photon_gathering_valid(false);
            Photonear::get_instance()->get_ray_tracer()->set_photon_map_available(false);
        }
    }
    if (pm_invalid) ImGui::PopStyleVar();

    ImGui::Separator();
    if (!pm_invalid) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    ImGui::DragInt("Max Depth Photon Mapping", &max_depth, 0.1, 1, INT_MAX);
    ImGui::DragInt("Photon by Lights by Pass", &photon_by_light_by_pass, 1, 1, INT_MAX);
    if (pm_invalid) {
        m_max_depth = max_depth;
        m_photon_by_light_by_pass = photon_by_light_by_pass;
    } else ImGui::PopStyleVar();
}

void PhotonMapper::compute_photon_mapping_pass() {
    m_num_pass++;
    m_photon_mapping_valid = true;
    m_photon_computed = 0;
    m_photon_mapping_computed = false;
    auto photonear = Photonear::get_instance();
    auto scene = photonear->get_scene();
    auto scene_graph = scene->get_scene_graph();
    auto ray_tracer = photonear->get_ray_tracer();

    auto lights_material = scene->get_lights();

    color default_color = ray_tracer->get_default_color();

    ray_tracer->set_photon_map_available(false);
    for (auto light_mat: lights_material) {
        auto light_node = Component::get_node(light_mat);
        auto trsf_comp = Component::get_component<TransformComponent>(&*light_node);
        auto matrix = trsf_comp->get_matrix_as_end_node();
        point origin = vec3(matrix * vec4(0, 0, 0, 1));

        for (int i = 0; i < m_photon_by_light_by_pass; i++) {
            if (!m_photon_mapping_valid) { //Cancel the execution
                return;
            }
            auto ray = light_mat->get_random_ray(matrix);
            std::async(&PhotonMapper::compute_ray_trace, // NOLINT(bugprone-unused-return-value
                       this, &*scene_graph, default_color, ray, light_mat);
        }
    }
    lock_photon_mapping();
    m_photon_map->build(m_photon_map_array);
    unlock_photon_mapping();
    m_photon_mapping_computed = true;
    ray_tracer->set_photon_map_available(true);
    photonear->get_ray_tracer()->set_photon_gathering_valid(false);
}

void PhotonMapper::compute_ray_trace(SceneGraph *scene_graph, color default_color, Ray ray,
                                     EmissiveMaterial *light_material) {
    auto ray_hit = scene_graph->raycast(ray, Material::REFRACTIVE_INDEX_AIR);
    light_material->alterate(ray_hit);
    if (ray_hit->hit) {
        auto node = Component::get_node(ray_hit->shape);
        auto material = Component::get_nearest_component_upper<Material>(&*node);
        material->resolve_ray(scene_graph, ray_hit, m_max_depth, default_color, true);
        compute_photon_trace(ray_hit);
    }
    m_nb_total_photons++;
    m_photon_computed++;
}

void PhotonMapper::compute_photon_trace(const std::shared_ptr<RayCastHit> &ray_hit) {
    if (ray_hit->contribute) {
        auto photon = std::make_shared<Photon>(ray_hit);
        photon->weight = photon->weight * EmissiveMaterial::DefaultIntensityForPhotonEmission;
        m_photon_map_array.push_back(photon);
    }
    for(const auto& bounce_ray : ray_hit->bounce_rays){
        compute_photon_trace(bounce_ray);
    }
}

void PhotonMapper::init_photon_map() {
    m_photon_mapping_valid = false;
    m_photon_map_array.clear();
}

void PhotonMapper::set_photon_mapping_valid(bool valid) {
    m_photon_mapping_valid = valid;
}

void PhotonMapper::generate_ui_logs() const {
    ImGui::Text("Photon Mapping");
    if (m_num_pass > 0) {
        ImGui::SameLine();
        ImGui::Text("Pass %d", m_num_pass);
    }
    if (m_photon_mapping_computed) {
        ImGui::ProgressBar(1);
    } else {
        ImGui::ProgressBar((float) m_photon_computed / ((float) m_photon_by_light_by_pass *
                                                        (float) Photonear::get_instance()->get_scene()->get_nb_lights()));
    }
    ImGui::Separator();
}

std::shared_ptr<PhotonMap> PhotonMapper::get_photon_map() {
    return m_photon_map;
}

int PhotonMapper::get_nb_total_photons() const {
    return m_nb_total_photons;
}

void PhotonMapper::set_pending_ray_tracing(bool pending) {
    m_pending_ray_tracing = pending;
}

void PhotonMapper::reinit_count_pass() {
    if (m_photon_mapping_valid) {
        m_num_pass = 1;
    } else {
        m_num_pass = 0;
    }
}

void PhotonMapper::lock_photon_mapping() {
    m_photon_mapping_mutex.lock();
}

void PhotonMapper::unlock_photon_mapping() {
    m_photon_mapping_mutex.unlock();
}
