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
    float bias = 0.001f;
    m_min = {m_min[0] - bias, m_min[1] - bias, m_min[2] - bias};
    m_max = {m_max[0] + bias, m_max[1] + bias, m_max[2] + bias};
}

void BoundingBox::draw() {
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
        auto origin = ray.get_origin();
        auto direction = ray.get_direction();
        auto t_min = ray.get_t_min();
        auto t_max = ray.get_t_max();
        auto invD = 1.0f / direction[i];
        auto t0 = (m_min[i] - origin[i]) * invD;
        auto t1 = (m_max[i] - origin[i]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;
        if (t_max <= t_min)
            return false;
    }
    return true;
}