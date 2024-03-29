//
// Created by mariusjenin on 29/11/22.
//
#include <iostream>
#include <string>
#include <climits>
#include "imgui.h"

#include "RayTracer.h"
#include "Photonear.h"
#include "Camera.h"
#include "RefractiveMaterial.h"

using namespace ray_tracing;
using namespace component::material;

RayTracer::RayTracer() {
    m_sample_by_pixel = 1;
    m_auto_size = false;
    m_image = {};
    m_ray_trace_data = {};
    m_auto_recompute = false;
    m_ray_tracing_valid = false;
    m_image_valid = false;
    m_photon_gathering_valid = true;
    m_is_ray_tracing = false;
    m_is_photon_gathering = false;
    m_photon_map_available = false;
    m_px_ray_traced = 0;
    m_ray_photon_gathered = 0;
    m_total_ray_photon_gathered = 0;
    m_width = 125;
    m_height = 100;
    m_default_color = {0, 0, 0};
    m_max_depth = 10;
    m_first_photon_gathering = false;
    m_radius_photon_gathering = 2.5;
    m_radius_reduce_proportion = 0.5;
}


void RayTracer::init() {
    init_ray_tracing_data();

    glGenTextures(1, &m_image_texture);
    glBindTexture(GL_TEXTURE_2D, m_image_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void RayTracer::generate_ui_ray_tracing_settings() {
    int width = m_width;
    int height = m_height;
    int sample_by_pixel = m_sample_by_pixel;
    int max_depth = m_max_depth;


    ImGui::Checkbox("Auto recompute Ray Tracing", &m_auto_recompute);
    if (ImGui::Button("Recompute")) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
        init_ray_tracing_data();
//        Photonear::get_instance()->get_photon_mapper()->reinit();
        if (m_async_ray_tracing.valid()) m_async_ray_tracing.wait();
        m_async_ray_tracing = std::async(&RayTracer::compute_ray_tracing_pass, this);
#pragma clang diagnostic pop
    }
    ImGui::Separator();


    color default_color = m_default_color;
    ImGui::ColorEdit3("Default Color", &default_color[0]);
    ImGui::DragInt("Max Depth Ray Tracing", &max_depth, 0.1, 1, INT_MAX);
    bool default_color_changed = m_default_color != default_color;
    bool max_depth_changed = m_max_depth != max_depth;
    m_default_color = default_color;
    m_max_depth = max_depth;

    ImGui::Separator();


    ImGui::Checkbox(("Use " + std::string(Photonear::PhotonMappingViewerName) + " size").c_str(), &m_auto_size);
    ImGui::Begin(Photonear::PhotonMappingViewerName, nullptr, ImGuiWindowFlags_NoMove);
    ImVec2 pmv_window_size = ImGui::GetWindowSize();
    ImGui::End();
    std::string display_width = "%d";
    std::string display_height = "%d";

    if (m_auto_size) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        display_width = std::to_string((int) pmv_window_size.x);
        display_height = std::to_string((int) pmv_window_size.y);
    }

    ImGui::DragInt("Width Render", &width, 1, 1, INT_MAX, display_width.c_str());
    ImGui::DragInt("Height Render", &height, 1, 1, INT_MAX, display_height.c_str());

    bool width_changed;
    bool height_changed;
    m_image_mutex.lock();
    if (m_auto_size) {
        ImGui::PopStyleVar();
        width_changed = m_width != (int) pmv_window_size.x;
        height_changed = m_height != (int) pmv_window_size.y;
        m_width = (int) pmv_window_size.x;
        m_height = (int) pmv_window_size.y;
    } else {
        width_changed = m_width != width;
        height_changed = m_height != height;
        m_width = width;
        m_height = height;
    }

    ImGui::DragInt("Sample by pixel", &sample_by_pixel, 0.1, 1, INT_MAX);
    bool sample_by_pixel_changed = m_sample_by_pixel != sample_by_pixel;
    m_sample_by_pixel = sample_by_pixel;
    if (width_changed || height_changed || sample_by_pixel_changed || default_color_changed || max_depth_changed) {
        init_ray_tracing_data();
    }
    m_image_mutex.unlock();
}

void RayTracer::generate_ui_photon_gathering_settings() {
    float radius_photon_gathering = m_radius_photon_gathering;
    ImGui::DragFloat("Radius Photon Gathering", &radius_photon_gathering, 0.001f, 0, FLT_MAX);
    bool radius_photon_gathering_changed = m_radius_photon_gathering != radius_photon_gathering;
    m_radius_photon_gathering = radius_photon_gathering;
    if (radius_photon_gathering_changed) on_radius_changed();

    bool reduce_proportion_editable = !m_is_photon_gathering;
    if(!reduce_proportion_editable) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    float reduce_proportion = m_radius_reduce_proportion;
    ImGui::SliderFloat("Radius reduce proportion", &reduce_proportion, 0.01f, 0.990f);
    bool reduce_proportion_changed = m_radius_reduce_proportion != reduce_proportion;
    if(reduce_proportion_editable) m_radius_reduce_proportion = reduce_proportion;
    if (reduce_proportion_changed) on_radius_changed();
    if(!reduce_proportion_editable) ImGui::PopStyleVar();
}

void RayTracer::init_ray_tracing_data() {
    set_ray_tracing_valid(false);
    m_ray_tracing_mutex.lock();
    int size_data = m_width * m_height * 4;
    m_ray_trace_data.clear();
    m_image.clear();
    m_image.resize(size_data, 0);
    m_ray_tracing_mutex.unlock();
}

void RayTracer::on_photon_mapping_reinit() {
    m_first_photon_gathering = true;
    for(const auto& ray_trace_hit : m_ray_trace_data){
        ray_trace_hit->flux = {0,0,0};
        ray_trace_hit->radius = m_radius_photon_gathering;
    }
}

void RayTracer::on_radius_changed() {
    m_ray_tracing_mutex.lock();
    auto photon_mapper = Photonear::get_instance()->get_photon_mapper();
    m_first_photon_gathering = true;
    photon_mapper->set_photon_mapping_valid(false);
    photon_mapper->reinit_count_pass();
    int ray_trace_size = (int) m_ray_trace_data.size();
    for (int i = 0; i < ray_trace_size; i++) {
        m_ray_trace_data[i]->radius = m_radius_photon_gathering;
    }
    m_ray_tracing_mutex.unlock();
}

void RayTracer::update() {
    if (m_auto_recompute && !m_ray_tracing_valid) {
        init_ray_tracing_data();
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
        m_async_ray_tracing = std::async(&RayTracer::compute_ray_tracing_pass, this);
#pragma clang diagnostic pop
    }
    if (!m_photon_gathering_valid && m_ray_tracing_valid && m_photon_map_available && !m_is_photon_gathering && !m_is_ray_tracing) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
        m_async_ray_tracing = std::async(&RayTracer::compute_photon_gathering, this);
#pragma clang diagnostic pop
    }
    if (!m_image_valid && m_ray_tracing_valid && m_photon_gathering_valid && !m_first_photon_gathering && !m_is_ray_tracing && !m_is_photon_gathering) {
        compute_image();
    }
}

void RayTracer::generate_ui_viewer() {
    auto window_size = ImGui::GetWindowSize();
    if (m_image_valid || (m_ray_tracing_valid && m_is_ray_tracing)) {
        auto cursor_pos = ImGui::GetCursorScreenPos();
        glBindTexture(GL_TEXTURE_2D, m_image_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_image[0]);
        float diff_x = 0;
        float diff_y = 0;
        float ratio_img = (float) m_width / (float) m_height;
        float inv_ratio_img = (float) m_height / (float) m_width;
        float ratio_window = window_size.x / window_size.y;

        if (ratio_img > ratio_window) {
            diff_y = window_size.y - (window_size.x * inv_ratio_img);
        }
        if (ratio_window > ratio_img) {
            diff_x = window_size.x - (window_size.y * ratio_img);
        }

        ImGui::GetWindowDrawList()->AddImage(
                reinterpret_cast<ImTextureID>(m_image_texture),
                ImVec2(cursor_pos.x + diff_x / 2.f, cursor_pos.y + diff_y / 2.f),
                ImVec2(cursor_pos.x + window_size.x - 15 - diff_x / 2.f,
                       cursor_pos.y + window_size.y - 35 - diff_y / 2.f),
                ImVec2(0, 1), ImVec2(1, 0)
        );
    } else {
        std::string text = "No Render";
        auto text_size = ImGui::CalcTextSize(text.c_str());
        ImGui::SetCursorPos(ImVec2((window_size.x - text_size.x) * 0.5f, (window_size.y - text_size.y) * 0.5f));
        ImGui::Text("%s", text.c_str());
    }
}

void RayTracer::compute_ray_tracing_pass() {
    auto scene = Photonear::get_instance()->get_scene();
    auto scene_graph = scene->get_scene_graph();
    std::shared_ptr<Camera> camera = scene->get_active_camera();
    if (camera == nullptr) {
        return;
    }

    set_ray_tracing_valid(true);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    m_is_ray_tracing = true;
#pragma clang diagnostic pop
    m_px_ray_traced = 0;
    m_ray_photon_gathered = 0;

    m_ray_tracing_valid = true;
    m_image_valid = true;
    m_first_photon_gathering = true;

    auto camera_node = Component::get_node(&*camera);
    auto trsf_comp = Component::get_component<TransformComponent>(&*camera_node);
    auto matrix = trsf_comp->get_matrix_as_end_node();

    float tan_half_fov = tan(glm::radians(camera->get_fovy() / 2.0f));

    float aspect_ratio = (float) m_width / (float) m_height;
    float z_near = camera->get_z_near();
    float z_far = camera->get_z_far();
    point origin = vec3(matrix * vec4(CAMERA_POSITION, 1));
    versor forward = vec3(matrix * vec4(CAMERA_FORWARD, 0));
    versor up = vec3(matrix * vec4(CAMERA_UP, 0));

    for (int j = m_height - 1; j >= 0; j--) {
        for (int i = 0; i < m_width; i++) {
            for (int k = 0; k < m_sample_by_pixel; k++) {
                if (!m_ray_tracing_valid) { //Cancel the execution
                    m_is_ray_tracing = false;
                    return;
                }
                float offset = (float) k / (float) m_sample_by_pixel / 2;
                if (k % 2 == 0) offset *= -1;
                float x = (2.0f * ((float) i + offset) / (float) m_width) - 1.0f;
                float y = (2.0f * ((float) j + offset) / (float) m_height) - 1.0f;
                x *= aspect_ratio;
                versor direction = glm::normalize(
                        forward + y * tan_half_fov * up + x * tan_half_fov * glm::cross(forward, up));
                Ray ray = Ray(origin, direction, z_near, z_far);
                std::async(&RayTracer::compute_ray_cast, this, // NOLINT(bugprone-unused-return-value
                           &*scene_graph, i, j, ray);
            }
        }
    }
    m_photon_gathering_valid = false;
    m_is_ray_tracing = false;
}

void RayTracer::compute_ray_cast(SceneGraph *scene_graph, int u, int v, Ray ray) {
    m_image_mutex.lock();
    m_ray_tracing_mutex.lock();

    color px_color;
    auto index = 4 * (v * m_width + u);

    auto ray_hit = scene_graph->raycast(ray, Material::REFRACTIVE_INDEX_AIR);
    ray_hit->weight = 1.f / (float) m_sample_by_pixel;
    if (ray_hit->hit) {
        auto node = Component::get_node(ray_hit->shape);
        auto material = &*Component::get_nearest_component_upper<Material>(&*node);
        if(material == nullptr) material = Material::get_default();
        px_color = material->resolve_ray(scene_graph, ray_hit, m_max_depth, m_default_color, false) /
                   (float) m_sample_by_pixel;
        compute_ray_trace(u, v, ray_hit);
    } else {
        px_color = m_default_color;
    }
    m_image[index] += (unsigned char) (px_color[0] * 255);
    m_image[index + 1] += (unsigned char) (px_color[1] * 255);
    m_image[index + 2] += (unsigned char) (px_color[2] * 255);
    m_image[index + 3] += 255;

    m_ray_tracing_mutex.unlock();
    m_image_mutex.unlock();
    m_px_ray_traced++;
}

void RayTracer::compute_ray_trace(int u, int v, const std::shared_ptr<RayCastHit> &ray_hit) {
    if (!ray_hit->hit) {
        auto ray_trace_hit = std::make_shared<RayTraceHit>(u, v, m_radius_photon_gathering);
        ray_trace_hit->color_in_buffer = m_default_color * ray_hit->attenuation;
        m_ray_trace_data.push_back(ray_trace_hit);
        return;
    }
    if (ray_hit->contribute) {
        auto ray_trace_hit = std::make_shared<RayTraceHit>(u, v, m_radius_photon_gathering);
        ray_trace_hit->hit = true;
        ray_trace_hit->shape = ray_hit->shape;
        ray_trace_hit->weight = ray_hit->weight;
        ray_trace_hit->attenuation = ray_hit->attenuation;
        ray_trace_hit->normal = ray_hit->normal;
        ray_trace_hit->hit_point = ray_hit->hit_point;
        ray_trace_hit->direction = ray_hit->direction;
        m_ray_trace_data.push_back(ray_trace_hit);
    }
    for(const auto& bounce_ray : ray_hit->bounce_rays){
        compute_ray_trace(u, v, bounce_ray);
    }
}

void RayTracer::compute_image() {
    m_image_mutex.lock();
    m_image.clear();
    m_image.resize(m_width * m_height * 4, 0);
    for (const auto &ray_trace_hit: m_ray_trace_data) {
        auto index = 4 * (ray_trace_hit->px_v * m_width + ray_trace_hit->px_u);
        color px_color = ray_trace_hit->color_in_buffer * ray_trace_hit->weight;
        m_image[index] += (unsigned char) (px_color[0] * 255);
        m_image[index + 1] += (unsigned char) (px_color[1] * 255);
        m_image[index + 2] += (unsigned char) (px_color[2] * 255);
        m_image[index + 3] += 255;
    }
    m_image_valid = true;
    m_image_mutex.unlock();
}

void RayTracer::compute_photon_gathering() {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    m_is_photon_gathering = true;
#pragma clang diagnostic pop
    m_ray_photon_gathered = 0;
    m_photon_gathering_valid = true;
    m_total_ray_photon_gathered = (int) m_ray_trace_data.size();
    auto photon_mapper = Photonear::get_instance()->get_photon_mapper();
    auto photon_map = photon_mapper->get_photon_map();
    int nb_total_photons =photon_mapper->get_nb_total_photons();
    for (const auto &ray_trace_hit: m_ray_trace_data) {
        if (!m_photon_gathering_valid) { //Cancel the execution
            m_is_photon_gathering = false;
            m_ray_photon_gathered = 0;
            return;
        }
        if(ray_trace_hit->hit){
            photon_mapper->lock_photon_mapping();
            if (m_first_photon_gathering) {
                photon_gathering(ray_trace_hit, photon_map,nb_total_photons);
            } else {
                refine_photon_gathering(ray_trace_hit, photon_map,nb_total_photons);
            }
            photon_mapper->unlock_photon_mapping();
        }
        m_ray_photon_gathered++;
    }
    m_first_photon_gathering = false;
    m_image_valid = false;
    Photonear::get_instance()->get_photon_mapper()->set_photon_mapping_valid(false);

    m_is_photon_gathering = false;
}

void RayTracer::photon_gathering(const std::shared_ptr<RayTraceHit> &ray_trace_hit,
                                 const std::shared_ptr<PhotonMap> &photon_map, int nb_total_photons) {
    m_ray_tracing_mutex.lock();
    ray_trace_hit->color_in_buffer = {0, 0, 0};

    auto photons_index = photon_map->neighbors_in_radius_search(ray_trace_hit->hit_point, ray_trace_hit->radius);
    for (auto photon_index: photons_index) {
        auto photon = photon_map->get(photon_index);
        float distance = length(photon->position - ray_trace_hit->hit_point);
        if (ray_trace_hit->shape->normal_test(ray_trace_hit,photon)) {
            ray_trace_hit->nb_photons++;
            ray_trace_hit->flux += photon->weight * photon->color_photon * (1 - distance / (1.1f * ray_trace_hit->radius));
        }
    }

    float pi_radius_squared = ((float) M_PI * ray_trace_hit->radius * ray_trace_hit->radius);

    ray_trace_hit->color_in_buffer = ray_trace_hit->flux * ray_trace_hit->attenuation / pi_radius_squared / (float)nb_total_photons;

    if (ray_trace_hit->color_in_buffer[0] > 1) ray_trace_hit->color_in_buffer[0] = 1;
    if (ray_trace_hit->color_in_buffer[1] > 1) ray_trace_hit->color_in_buffer[1] = 1;
    if (ray_trace_hit->color_in_buffer[2] > 1) ray_trace_hit->color_in_buffer[2] = 1;

    m_ray_tracing_mutex.unlock();
}

void RayTracer::refine_photon_gathering(const std::shared_ptr<RayTraceHit> &ray_trace_hit,
                                        const std::shared_ptr<PhotonMap> &photon_map, int nb_total_photons) {
    m_ray_tracing_mutex.lock();
    color new_flux = {0, 0, 0};

    std::vector<std::pair<std::shared_ptr<Photon>,float>> new_photons = {};
    auto photons_index = photon_map->neighbors_in_radius_search(ray_trace_hit->hit_point, ray_trace_hit->radius);
    for(auto photon_index : photons_index) {
        auto photon = photon_map->get(photon_index);
        float distance = length(photon->position - ray_trace_hit->hit_point);
        if (ray_trace_hit->shape->normal_test(ray_trace_hit,photon)) {
            new_photons.emplace_back(photon, distance);
        }
    }

    for(const auto& photon_pair : new_photons) {
        auto photon = photon_pair.first;
        auto distance = photon_pair.second;
        new_flux += photon->weight * photon->color_photon * (1 - distance / ray_trace_hit->radius);
    }


    int new_photons_size = (int)new_photons.size();
    float frac_pm = ((float)ray_trace_hit->nb_photons + m_radius_reduce_proportion * (float)new_photons_size) / ((float)ray_trace_hit->nb_photons + (float)new_photons_size);
    float pi_radius_squared = ((float)M_PI*ray_trace_hit->radius*ray_trace_hit->radius);

    ray_trace_hit->nb_photons+=new_photons_size;
    ray_trace_hit->flux = (ray_trace_hit->flux + new_flux) * frac_pm;
    ray_trace_hit->color_in_buffer = ray_trace_hit->flux *  ray_trace_hit->attenuation / pi_radius_squared/ (float)nb_total_photons;
    ray_trace_hit->radius = ray_trace_hit->radius * sqrt(frac_pm);

    if (ray_trace_hit->color_in_buffer[0] > 1) ray_trace_hit->color_in_buffer[0] = 1;
    if (ray_trace_hit->color_in_buffer[1] > 1) ray_trace_hit->color_in_buffer[1] = 1;
    if (ray_trace_hit->color_in_buffer[2] > 1) ray_trace_hit->color_in_buffer[2] = 1;
    m_ray_tracing_mutex.unlock();
}

void RayTracer::set_ray_tracing_valid(bool valid) {
    auto photon_mapper = Photonear::get_instance()->get_photon_mapper();
    if (!valid) {
        if(m_auto_recompute){
            m_image_valid = false;
        }
        if(m_ray_tracing_valid){
            m_photon_gathering_valid = false;
        }
        m_px_ray_traced = 0;
        m_ray_photon_gathered = 0;
        if (photon_mapper != nullptr){
            photon_mapper->reinit_count_pass();
        }
    }
    if (photon_mapper != nullptr)
        photon_mapper->set_pending_ray_tracing(!valid);

    m_ray_tracing_valid = valid;
}

void RayTracer::set_photon_gathering_valid(bool valid) {
    m_photon_gathering_valid = valid;
    if (!valid) {
        m_ray_photon_gathered = 0;
    }
}


void RayTracer::generate_ui_ray_tracing_running_tasks() const {
    ImGui::Text("Ray Tracing");
    ImGui::ProgressBar((float) m_px_ray_traced / ((float) m_width * (float) m_height * (float) m_sample_by_pixel));
    ImGui::Separator();
}

void RayTracer::generate_ui_ray_tracing_logs() const {
    ImGui::Text("Ray Tracing");
    std::string ray_sent = "Ray sent :          ";
    int px_total = m_width * m_height * m_sample_by_pixel;
    ray_sent.append(std::to_string(m_px_ray_traced)+"/"+std::to_string(px_total));
    ImGui::Text("%s", ray_sent.c_str());
    std::string ray_saved = "Ray saved :         ";
    ray_saved.append(std::to_string(m_ray_trace_data.size()));
    ImGui::Text("%s", ray_saved.c_str());
}


void RayTracer::generate_ui_photon_gathering_logs() const {
    ImGui::Text("Photon Gathering");
    std::string photon_gathered = "Photon gathered :   ";
    photon_gathered.append(std::to_string(m_ray_photon_gathered)+"/"+std::to_string(m_total_ray_photon_gathered));
    ImGui::Text("%s", photon_gathered.c_str());
}


void RayTracer::generate_ui_photon_gathering_running_tasks() const {
    ImGui::Text("Photon Gathering");
    if(m_total_ray_photon_gathered == 0){
        ImGui::ProgressBar(0);
    } else{
        ImGui::ProgressBar((float) m_ray_photon_gathered / ((float) m_total_ray_photon_gathered));
    }
    ImGui::Separator();
}

color RayTracer::get_default_color() {
    return m_default_color;
}

void RayTracer::set_photon_map_available(bool available) {
    m_photon_map_available = available;
}
