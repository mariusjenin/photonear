//
// Created by mariusjenin on 29/11/22.
//

#include <string>
#include <climits>
#include "imgui.h"

#include "RayTracer.h"
#include "Photonear.h"
#include "Camera.h"

using namespace ray_tracing;

RayTracer::RayTracer() {
    m_sample_by_pixel = 1;
    m_auto_size = false;
    m_data = {};
    m_photon_hit = {};
    m_auto_recompute = false;
    m_ray_tracing_valid = false;
    m_image_valid = false;
    m_is_computing = false;
    m_px_computed = 0;
    m_width = 120;
    m_height = 80;
}


void RayTracer::init() {
    init_ray_tracing_data();

    glGenTextures(1, &m_image_texture);
    glBindTexture(GL_TEXTURE_2D, m_image_texture);

    // Setup filtering parameters for display
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

    m_image_size_mutex.lock();
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
    m_image_size_mutex.unlock();

    ImGui::Separator();
    ImGui::Checkbox("Auto recompute Ray Tracing", &m_auto_recompute);
    if (width_changed || height_changed || sample_by_pixel_changed ) {
        init_ray_tracing_data();
    }
    m_image_size_mutex.unlock();
    if (ImGui::Button("Recompute")) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
        m_async_ray_tracing = std::async(&RayTracer::compute_raytracing, this);
#pragma clang diagnostic pop
    }
}

void RayTracer::init_ray_tracing_data() {
    m_ray_tracing_valid = false;
    int size_img = m_width * m_height;
    int size_data = size_img * 4;
    m_data.clear();
    m_data.resize(size_data, 0);
    m_photon_hit.resize(size_img, {});
}

void RayTracer::update() {
    if (m_auto_recompute && !m_ray_tracing_valid) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
        init_ray_tracing_data();
        m_async_ray_tracing = std::async(&RayTracer::compute_raytracing, this);
#pragma clang diagnostic pop
    }
    if (!m_image_valid && m_ray_tracing_valid) {
        compute_image();
    }

}

void RayTracer::generate_ui_viewer() {
    auto window_size = ImGui::GetWindowSize();
    if (m_image_valid) {
        auto cursor_pos = ImGui::GetCursorScreenPos();
        glBindTexture(GL_TEXTURE_2D, m_image_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_data[0]);
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

void RayTracer::compute_raytracing() {
    m_image_valid = false;
    auto scene = Photonear::get_instance()->get_scene();
    auto scene_graph = scene->get_scene_graph();
    std::shared_ptr<Camera> camera = scene->get_active_camera();
    if (camera == nullptr) {
        return;
    }

    m_ray_tracing_valid = true;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    m_is_computing = true;
#pragma clang diagnostic pop
    m_px_computed = 0;


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
                m_is_computing = false;
                m_image_size_mutex.unlock();
                return;
            }
            float x = (2.0f * (float) i / (float) m_width) - 1.0f;
            float y = (2.0f * (float) j / (float) m_height) - 1.0f;
            x *= aspect_ratio;
            versor direction = glm::normalize( forward + y * tan_half_fov * up + x * tan_half_fov * glm::cross(forward, up));
            std::async(&RayTracer::compute_raytracing_ray, this, scene_graph, i, j, origin, direction, z_near, z_far); // NOLINT(bugprone-unused-return-value)
        }
    }
    m_is_computing = false;
}

void RayTracer::compute_raytracing_ray(const std::shared_ptr<SceneGraph> &scene_graph, int x, int y, vec3 origin,
                                       vec3 direction, float z_near, float z_far) {
    m_image_size_mutex.lock();

    Ray ray = Ray(origin, direction, z_near, z_far);
    auto index = 4 * (y * m_width + x);
    if (scene_graph->raycast(ray)) {
        m_data[index] = 128;
        m_data[index + 1] = 128;
        m_data[index + 2] = 128;
    } else {
        m_data[index] = 64;
        m_data[index + 1] = 64;
        m_data[index + 2] = 64;
    }
    m_data[index + 3] = 255;

    m_image_size_mutex.unlock();
    m_px_computed++;
}

void RayTracer::compute_image() {
    //TODO
    m_image_valid = true;
}

void RayTracer::set_ray_tracing_valid(bool computed) {
    m_ray_tracing_valid = computed;
}

bool RayTracer::is_ray_tracing_valid() const {
    return m_ray_tracing_valid;
}

void RayTracer::generate_ui_logs() const {
    ImGui::Text("Ray Tracing");
    ImGui::SameLine();
    if (m_is_computing) {
        ImGui::Text("Computing");
        ImGui::ProgressBar(m_px_computed / ((float) m_width * (float) m_height));
    } else {
        if (m_ray_tracing_valid) {
            ImGui::Text("Computed");
        } else {
            ImGui::Text("Not Computed");
        }
    }
    ImGui::Separator();
}
