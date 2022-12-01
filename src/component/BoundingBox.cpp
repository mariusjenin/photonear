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

void BoundingBox::merge(const std::vector<glm::vec3> &vertices) {
    for (auto vertex: vertices) {
        for (int i = 0; i < 3; i++) {
            if (m_min[i] > vertex[i]) m_min[i] = vertex[i];
            if (m_max[i] < vertex[i]) m_max[i] = vertex[i];
        }
    }
}

void BoundingBox::generate_ui_component_editor() {
    ImGui::Text("min %f %f %f", m_min[0], m_min[1], m_min[2]);
    ImGui::Text("max %f %f %f", m_max[0], m_max[1], m_max[2]);
}

void BoundingBox::draw(const std::shared_ptr<Shaders> &shaders) {
    auto scene = Photonear::get_instance()->get_scene();
    auto shader_data_manager = shaders->get_shader_data_manager();
    if (scene->is_debug_enabled()) {
        glm::vec3 m_color_rendering = scene->get_debug_color();

        glUniform1i(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_LOC_NAME), true);
        glUniform3fv(shader_data_manager->get_location(ShadersDataManager::DEBUG_RENDERING_COLOR_LOC_NAME),
                     1, &m_color_rendering[0]);
        std::vector<glm::vec3> vertices = to_vertices();
        std::vector<glm::vec3> lines = {
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
}

std::vector<glm::vec3> BoundingBox::to_vertices() {
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