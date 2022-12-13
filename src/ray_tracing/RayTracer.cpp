//
// Created by mariusjenin on 29/11/22.
//
#include <string>
#include <climits>
#include "imgui.h"

#include "RayTracer.h"
#include "Photonear.h"
#include "Camera.h"
#include "DielectricMaterial.h"

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
    m_photon_splatting_valid = true;
    m_is_ray_tracing = false;
    m_is_photon_splatting = false;
    m_px_ray_traced = 0;
    m_ray_photon_splatted = 0;
    m_total_ray_photon_splatted = 0;
    m_width = 125;
    m_height = 100;
    m_default_color = {0, 0, 0};
    m_max_depth = 3;
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
    bool width_changed;
    bool height_changed;
    bool sample_by_pixel_changed;
    int width = m_width;
    int height = m_height;
    int sample_by_pixel = m_sample_by_pixel;


    ImGui::Checkbox("Auto recompute Ray Tracing", &m_auto_recompute);
    if (ImGui::Button("Recompute")) {
        #pragma clang diagnostic push
        #pragma ide diagnostic ignored "UnusedValue"
        init_ray_tracing_data();
        if(m_async_ray_tracing.valid())
            m_async_ray_tracing.wait();
        m_async_ray_tracing = std::async(&RayTracer::compute_ray_tracing_pass, this);
        #pragma clang diagnostic pop
    }
    ImGui::Separator();


    color default_color = m_default_color;
    int max_depth = m_max_depth;
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
    ImGui::DragInt("Sample by pixel", &sample_by_pixel, 0.1, 1, INT_MAX);
    sample_by_pixel_changed = m_sample_by_pixel != sample_by_pixel;
    m_sample_by_pixel = sample_by_pixel;

    m_ray_tracing_mutex.lock();
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

    if (width_changed || height_changed || sample_by_pixel_changed || default_color_changed || max_depth_changed) {
        init_ray_tracing_data();
    }
    m_ray_tracing_mutex.unlock();
}

void RayTracer::init_ray_tracing_data() {
    set_ray_tracing_valid(false);
    int size_data = m_width * m_height * 4;
    m_ray_trace_data.clear();
    m_image.clear();
    m_image.resize(size_data, 0);
}

void RayTracer::update() {
    if (m_auto_recompute && !m_ray_tracing_valid) {
        init_ray_tracing_data();
        #pragma clang diagnostic push
        #pragma ide diagnostic ignored "UnusedValue"
        m_async_ray_tracing = std::async(&RayTracer::compute_ray_tracing_pass, this);
        #pragma clang diagnostic pop
    }
    if (!m_photon_splatting_valid && m_ray_tracing_valid && !m_is_photon_splatting && !m_is_ray_tracing) {
        #pragma clang diagnostic push
        #pragma ide diagnostic ignored "UnusedValue"
        m_async_ray_tracing = std::async(&RayTracer::compute_photon_splatting, this);
        #pragma clang diagnostic pop
    }
    if(!m_image_valid && m_ray_tracing_valid && m_photon_splatting_valid && !m_is_ray_tracing && !m_is_photon_splatting){
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
    Photonear::get_instance()->get_photon_mapper()->set_photon_mapping_valid(false);
    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "UnusedValue"
    m_is_ray_tracing = true;
    #pragma clang diagnostic pop
    m_px_ray_traced = 0;


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
            if (!m_ray_tracing_valid) { //Cancel the execution
                m_is_ray_tracing = false;
                return;
            }
            float x = (2.0f * (float) i / (float) m_width) - 1.0f;
            float y = (2.0f * (float) j / (float) m_height) - 1.0f;
            x *= aspect_ratio;
            versor direction = glm::normalize(
                    forward + y * tan_half_fov * up + x * tan_half_fov * glm::cross(forward, up));
            std::async(&RayTracer::compute_ray_cast, this, // NOLINT(bugprone-unused-return-value
                       &*scene_graph, i, j, origin, direction, z_near, z_far);
        }
    }
    m_is_ray_tracing = false;
    m_image_valid = true;
}

void RayTracer::compute_ray_cast(SceneGraph* scene_graph, int u, int v, vec3 origin,
                                 vec3 direction, float z_near, float z_far) {
    m_ray_tracing_mutex.lock();

    color px_color;
    auto index = 4 * (v * m_width + u);

    Ray ray = Ray(origin, direction, z_near, z_far);
    auto ray_hit = scene_graph->raycast(ray, Material::REFRACTIVE_INDEX_AIR);
    if(ray_hit->hit){
        auto node = Component::get_node(ray_hit->shape);
        auto material = Component::get_nearest_component_upper<Material>(&*node);
        px_color = material->resolve_ray(scene_graph,ray_hit,m_max_depth,m_default_color, false);
        compute_ray_trace(u, v, ray_hit);
    } else{
        px_color =  m_default_color;
    }

    m_image[index] = (unsigned char) (px_color[0] * 255);
    m_image[index + 1] = (unsigned char) (px_color[1] * 255);
    m_image[index + 2] = (unsigned char) (px_color[2] * 255);
    m_image[index + 3] = 255;

    m_ray_tracing_mutex.unlock();
    m_px_ray_traced++;
}

void RayTracer::compute_ray_trace(int u, int v, const std::shared_ptr<RayCastHit>& ray_hit){
    auto children_ray_hit = ray_hit->children;
    if(!ray_hit->hit) {
        auto ray_trace_hit = std::make_shared<RayTraceHit>(u,v);
        ray_trace_hit->color_in_buffer = m_default_color * ray_hit->attenuation;
        m_ray_trace_data.push_back(ray_trace_hit);
        return;
    }
    if(ray_hit->diffuse) {
        auto ray_trace_hit = std::make_shared<RayTraceHit>(u,v);
        ray_trace_hit->weight = ray_hit->weight;
        ray_trace_hit->attenuation = ray_hit->attenuation;
        ray_trace_hit->normal = ray_hit->normal;
        ray_trace_hit->hit_point = ray_hit->hit_point;
        ray_trace_hit->direction = ray_hit->direction;
        m_ray_trace_data.push_back(ray_trace_hit);
    }
    if(!children_ray_hit.empty()){
        for(const auto& child : children_ray_hit){
            compute_ray_trace(u, v, child);
        }
    }
}

void RayTracer::compute_image() {
    for(const auto& ray_trace_hit: m_ray_trace_data){
        if(ray_trace_hit->nb_photons == 0){
            auto index = 4 * (ray_trace_hit->px_v * m_width + ray_trace_hit->px_u);
            color px_color = ray_trace_hit->attenuation;
            m_image[index] = (unsigned char) (px_color[0] * 255);
            m_image[index + 1] = (unsigned char) (px_color[1] * 255);
            m_image[index + 2] = (unsigned char) (px_color[2] * 255);
            m_image[index + 3] = 255;
        } else {
            auto index = 4 * (ray_trace_hit->px_v * m_width + ray_trace_hit->px_u);
            color px_color = ray_trace_hit->color_in_buffer;
            m_image[index] = (unsigned char) (px_color[0] * 255);
            m_image[index + 1] = (unsigned char) (px_color[1] * 255);
            m_image[index + 2] = (unsigned char) (px_color[2] * 255);
            m_image[index + 3] = 255;
        }
    }
    m_image_valid = true;
    m_ray_photon_splatted = 0;
}

void RayTracer::compute_photon_splatting(){
    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "UnusedValue"
    m_is_photon_splatting = true;
    #pragma clang diagnostic pop
    m_ray_photon_splatted = 0;
    m_total_ray_photon_splatted = (int)m_ray_trace_data.size();
    auto photon_map = Photonear::get_instance()->get_photon_mapper()->get_photon_map();
    for(const auto& ray_trace_hit: m_ray_trace_data){
        photon_splatting(ray_trace_hit, photon_map);
        m_ray_photon_splatted++;
    }
    m_image_valid = false;
    m_photon_splatting_valid = true;
    Photonear::get_instance()->get_photon_mapper()->set_photon_mapping_valid(false);

    m_is_photon_splatting = false;
}

void RayTracer::photon_splatting(const std::shared_ptr<RayTraceHit>& ray_trace_hit, const std::vector<std::shared_ptr<Photon>>& photon_map){
    for(const auto& photon : photon_map) {
        if (length(photon->position - ray_trace_hit->hit_point) <= ray_trace_hit->radius) {
            ray_trace_hit->nb_photons++;
            ray_trace_hit->color_in_buffer += 0.002f*photon->weight * photon->color_photon * ray_trace_hit->attenuation
                    * dot(photon->normal,ray_trace_hit->normal);
        }
    }
}

void RayTracer::set_ray_tracing_valid(bool valid) {
    auto photon_mapper = Photonear::get_instance()->get_photon_mapper();
    m_ray_tracing_valid = valid;
    if(!valid) {
        if(m_auto_recompute){
            m_image_valid = false;
        }
        m_px_ray_traced = 0;
        if(photon_mapper != nullptr)
            photon_mapper->reinit_count_pass();
    }
    if(photon_mapper != nullptr)
        photon_mapper->set_pending_ray_tracing(!valid);
}

void RayTracer::set_photon_splatting_valid(bool valid) {
    m_photon_splatting_valid = valid;
    if(!valid){
        m_ray_photon_splatted = 0;
    }
}


void RayTracer::generate_ui_ray_tracing_logs() const {
    ImGui::Text("Ray Tracing");
    ImGui::ProgressBar((float)m_px_ray_traced / ((float) m_width * (float) m_height));
    ImGui::Separator();
}


void RayTracer::generate_ui_photon_splatting_logs() const {
    ImGui::Text("Photon Splatting");
    ImGui::ProgressBar((float)m_ray_photon_splatted / (float) m_total_ray_photon_splatted);
    ImGui::Separator();
}

color RayTracer::get_default_color() {
    return m_default_color;
}
