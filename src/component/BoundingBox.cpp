//
// Created by mariusjenin on 30/11/22.
//

#include "imgui.h"
#include "BoundingBox.h"
#include "Photonear.h"
#include "ShadersBufferManager.h"

using namespace component;

BoundingBox::BoundingBox() {
    init();
}

void BoundingBox::init() {
    m_min = {FLT_MAX, FLT_MAX, FLT_MAX};
    m_max = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
}

ComponentType BoundingBox::get_type() {
    return typeid(this);
}

void BoundingBox::merge(BoundingBox bb) {
    merge(bb.to_vertices());
}

void BoundingBox::merge(const std::vector<point> &vertices) {
    for (auto vertex: vertices) {
        for (int i = 0; i < 3; i++) {
            if (m_min[i] > vertex[i]) m_min[i] = vertex[i];
            if (m_max[i] < vertex[i]) m_max[i] = vertex[i];
        }
    }
    m_min = {m_min[0] - 0.001f,m_min[1] - 0.001f,m_min[2] - 0.001f};
    m_max = {m_max[0] + 0.001f,m_max[1] + 0.001f,m_max[2] + 0.001f};
}

void BoundingBox::generate_ui_component_editor() {
    ImGui::Text("min %f %f %f", m_min[0], m_min[1], m_min[2]);
    ImGui::Text("max %f %f %f", m_max[0], m_max[1], m_max[2]);
}

void BoundingBox::draw(const std::shared_ptr<Shaders> &shaders, color color) {
    auto shader_data_manager = shaders->get_shader_data_manager();
    glUniform1i(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_LOC_NAME), true);
    glUniform3fv(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_COLOR_LOC_NAME),
                 1, &color[0]);
    std::vector<point> vertices = to_vertices();
    std::vector<point> lines = {
            vertices[0], vertices[1],
            vertices[1], vertices[3],
            vertices[3], vertices[2],
            vertices[2], vertices[0],
            vertices[4], vertices[5],
            vertices[5], vertices[7],
            vertices[7], vertices[6],
            vertices[6], vertices[4],
            vertices[0], vertices[4],
            vertices[1], vertices[5],
            vertices[2], vertices[6],
            vertices[3], vertices[7],
    };

    ShadersBufferManager::draw_verticies_debug(GL_LINES, lines);

    glUniform1i(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_LOC_NAME), false);
}

std::vector<point> BoundingBox::to_vertices() {
    return {m_min,
            {m_min[0], m_min[1], m_max[2]},
            {m_min[0], m_max[1], m_min[2]},
            {m_min[0], m_max[1], m_max[2]},
            {m_max[0], m_min[1], m_min[2]},
            {m_max[0], m_min[1], m_max[2]},
            {m_max[0], m_max[1], m_min[2]},
            m_max
    };
}

bool BoundingBox::hit_by_ray(Ray ray) {

    for (int i = 0; i < 3; i++) {
//        auto invD = 1.0f / r.direction()[i];
//        auto t0 = (min()[i] - r.origin()[i]) * invD;
//        auto t1 = (max()[i] - r.origin()[i]) * invD;
//        if (invD < 0.0f)
//            std::swap(t0, t1);
//        t_min = t0 > t_min ? t0 : t_min;
//        t_max = t1 < t_max ? t1 : t_max;
//        if (t_max <= t_min)
//            return false;
    }
    return true;
}