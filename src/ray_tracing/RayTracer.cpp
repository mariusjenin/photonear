//
// Created by mariusjenin on 29/11/22.
//

#include <string>
#include <limits.h>
#include "imgui.h"

#include "RayTracer.h"
#include "Photonear.h"


ray_tracing::RayTracer::RayTracer() {
    m_sample_by_pixel = 1;
    m_auto_size = true;
    m_data = {};
    m_photon_hit = {};
    m_auto_recompute = true;
    m_ray_tracing_valid = false;
    m_image_valid = false;
}


void RayTracer::init() {
    ImGui::Begin(Photonear::PhotonMappingViewerName, nullptr, ImGuiWindowFlags_NoMove);
    ImVec2 pmv_window_size = ImGui::GetWindowSize();
    ImGui::End();

    m_width = (int) pmv_window_size.x;
    m_height = (int) pmv_window_size.y;

    init_ray_tracing_data();

    glGenTextures(1, &m_image_texture);
    glBindTexture(GL_TEXTURE_2D, m_image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void ray_tracing::RayTracer::generate_ui_ray_tracing_settings() {
    bool width_changed;
    bool height_changed;
    bool sample_by_pixel_changed;
    int width = m_width;
    int height = m_height;
    int sample_by_pixel = m_sample_by_pixel;

    ImGui::Checkbox(("Use " + std::string(Photonear::PhotonMappingViewerName) + " size").c_str(), &m_auto_size);
    ImGui::Begin(Photonear::PhotonMappingViewerName);
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
    sample_by_pixel_changed = m_sample_by_pixel != sample_by_pixel;
    m_sample_by_pixel = sample_by_pixel;
    ImGui::Separator();
    ImGui::Checkbox("Auto recompute Ray Tracing", &m_auto_recompute);
    if (width_changed || height_changed || sample_by_pixel_changed) {
        init_ray_tracing_data();
    }
    if (ImGui::Button("Recompute")) {
        compute_raytracing();
    }
}

void RayTracer::init_ray_tracing_data() {
    int size_img = m_width * m_height;
    int size_data = size_img * 4;
    m_data.resize(size_data);
//    for (int i = 0; i < size_data; i += 4) {
//        m_data[i + 0] = 0;
//        m_data[i + 1] = 0;
//        m_data[i + 2] = 0;
//        m_data[i + 3] = 255;
//    }
    for (int i = 0; i < m_width; i++) {
        for (int j = 0; j < m_height; j++) {
            int val = 0;
//            if(i % 50 < 25){
//                if(j % 50 < 25){
//                    val = 0;
//                } else {
//                    val = 128;
//                }
//            } else {
//                if(j % 50 < 25){
//                    val = 128;
//                } else {
//                    val = 0;
//                }
//            }
//            if(j % 50 < 25){
//                val = 0;
//            } else {
//                val = 128;
//            }
            m_data[4 * (i * m_height + j) + 0] = val;
            m_data[4 * (i * m_height + j) + 1] = val;
            m_data[4 * (i * m_height + j) + 2] = val;
            m_data[4 * (i * m_height + j) + 3] = 255;
        }
    }
    for (int i = 2 * m_width + 2 - 48; i < size_data; i += 4 * m_width) {
        for (int j = 0; j < 48; ++j) {
            m_data[i + j * 4] = 128;
            m_data[i + j * 4 + 1] = 128;
            m_data[i + j * 4 + 2] = 128;
            m_data[i + j * 4 + 3] = 128;
        }
    }
    for (int i = ((m_height - 48) / 2) * m_width * 4; i < ((m_height + 48) / 2) * m_width * 4; i += 4) {
        m_data[i] = 128;
        m_data[i + 1] = 128;
        m_data[i + 2] = 128;
        m_data[i + 3] = 128;
    }
    m_photon_hit.resize(size_img, {});
    m_ray_tracing_valid = false;
}

void RayTracer::update() {
    if (m_auto_recompute && !m_ray_tracing_valid) {
        compute_raytracing();
    }
    if (!m_image_valid) {
        compute_image();
    }
}

void RayTracer::generate_ui_viewer() {
    auto window_size = ImGui::GetWindowSize();
    auto cursor_pos = ImGui::GetCursorScreenPos();
    glBindTexture(GL_TEXTURE_2D, m_image_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_data[0]);

    float diff_x = 0;
    float diff_y = 0;

    if ((float) m_width > window_size.x) {
        diff_y = window_size.y - (window_size.x * (float) m_height / (float) m_width);
    }
    if ((float) m_height > window_size.y) {
        diff_x = window_size.x - (window_size.y * (float) m_width / (float) m_height);
    }

    ImGui::GetWindowDrawList()->AddImage(
            reinterpret_cast<ImTextureID>(m_image_texture),
            ImVec2(cursor_pos.x + diff_x / 2.f, cursor_pos.y + diff_y / 2.f),
            ImVec2(cursor_pos.x + window_size.x - 15 - diff_x / 2.f, cursor_pos.y + window_size.y - 35 - diff_y / 2.f),
            ImVec2(0, 1), ImVec2(1, 0)
    );
}

void RayTracer::compute_raytracing() {
    //TODO
    m_ray_tracing_valid = true;
    m_image_valid = false;
}

void RayTracer::compute_image() {
    //TODO
    m_image_valid = true;
}

void RayTracer::set_ray_tracing_valid(bool computed) {
    m_ray_tracing_valid = computed;
}