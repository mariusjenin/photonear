//
// Created by mariusjenin on 12/12/22.
//

#include "PhotonMap.h"

using namespace ray_tracing;

PhotonMap::PhotonMap() : m_root(nullptr) {}

PhotonMap::PhotonMap(const std::vector<std::shared_ptr<Photon>>& photons) : m_root(nullptr) { build(photons); }

PhotonMap::~PhotonMap() { clear(); }

void PhotonMap::build(const std::vector<std::shared_ptr<Photon>>& photons) {
    clear();

    m_photons = photons;

    std::vector<int> indices(photons.size());
    std::iota(std::begin(indices), std::end(indices), 0);

    m_root = build_recursive(indices.data(), (int) photons.size(), 0);
}

void PhotonMap::clear()  {
    clear_recursive(m_root);
    m_root = nullptr;
    m_photons.clear();
}

bool PhotonMap::validate() const {
    try {
        validate_recursive(m_root, 0);
    }
    catch (const std::exception &) {
        return false;
    }

    return true;
}

int PhotonMap::nearest_neighbor_search(point position, float *minDist) const {
    int guess;
    float _minDist = std::numeric_limits<float>::max();

    nearest_neighbor_search_recursive(position, m_root, &guess, &_minDist);

    if (minDist)
        *minDist = _minDist;

    return guess;
}

std::vector<int> PhotonMap::k_nearest_neighbors_search(point position, int k) const {
    KNNQueue queue(k);
    k_nearest_neighbor_search_recursive(position, m_root, queue, k);

    std::vector<int> indices(queue.size());
    for (size_t i = 0; i < queue.size(); i++)
        indices[i] = queue[i].second;

    return indices;
}

std::vector<int>
PhotonMap::neighbors_in_radius_search(point position, float radius) const {
    std::vector<int> indices;
    neighbors_in_radius_search_recursive(position, m_root, indices, radius);
    return indices;
}

PhotonMap::NodePhotonMap *PhotonMap::build_recursive(int *indices, int n_photons, int depth) {
    if (n_photons <= 0)
        return nullptr;

    const int axis = depth % 3;
    const int mid = (n_photons - 1) / 2;

    std::nth_element(indices, indices + mid, indices + n_photons, [&](int lhs, int rhs) {
        return m_photons[lhs]->position[axis] < m_photons[rhs]->position[axis];
    });

    auto *node = new NodePhotonMap();
    node->idx = indices[mid];
    node->axis = axis;

    node->next[0] = build_recursive(indices, mid, depth + 1);
    node->next[1] = build_recursive(indices + mid + 1, n_photons - mid - 1, depth + 1);

    return node;
}

void PhotonMap::clear_recursive(PhotonMap::NodePhotonMap *node) {
    if (node == nullptr)
        return;

    if (node->next[0])
        clear_recursive(node->next[0]);

    if (node->next[1])
        clear_recursive(node->next[1]);

    delete node;
}

void PhotonMap::validate_recursive(const PhotonMap::NodePhotonMap *node, int depth) const {
    if (node == nullptr)
        return;

    const int axis = node->axis;
    const NodePhotonMap *node0 = node->next[0];
    const NodePhotonMap *node1 = node->next[1];

    if (node0 && node1) {
        if (m_photons[node->idx]->position[axis] < m_photons[node0->idx]->position[axis])
            throw std::exception();

        if (m_photons[node->idx]->position[axis] > m_photons[node1->idx]->position[axis])
            throw std::exception();
    }

    if (node0)
        validate_recursive(node0, depth + 1);

    if (node1)
        validate_recursive(node1, depth + 1);
}

float PhotonMap::distance(const std::shared_ptr<Photon>& p, const std::shared_ptr<Photon>& q) {
    float dist = 0;
    for (int i = 0; i < 3; i++)
        dist += (p->position[i] - q->position[i]) * (p->position[i] - q->position[i]);
    return sqrt(dist);
}

float PhotonMap::distance(point position, const std::shared_ptr<Photon>& q) {
    float dist = 0;
    for (int i = 0; i < 3; i++)
        dist += (position[i] - q->position[i]) * (position[i] - q->position[i]);
    return sqrt(dist);
}

void PhotonMap::nearest_neighbor_search_recursive(point position,
                                                               const PhotonMap::NodePhotonMap *node,
                                                               int *guess, float *minDist) const {
    if (node == nullptr)
        return;

    std::shared_ptr<Photon> train = m_photons[node->idx];

    const float dist = distance(position, train);
    if (dist < *minDist) {
        *minDist = dist;
        *guess = node->idx;
    }

    const int axis = node->axis;
    const int dir = position[axis] < train->position[axis] ? 0 : 1;
    nearest_neighbor_search_recursive(position, node->next[dir], guess, minDist);

    const auto diff = (float)fabs(position[axis] - train->position[axis]);
    if (diff < *minDist)
        nearest_neighbor_search_recursive(position, node->next[!dir], guess, minDist);
}

void PhotonMap::k_nearest_neighbor_search_recursive(point position,
                                                                 const PhotonMap::NodePhotonMap *node,
                                                                 PhotonMap::KNNQueue &queue, int k) const {
    if (node == nullptr)
        return;

    std::shared_ptr<Photon> train = m_photons[node->idx];

    const float dist = distance(position, train);
    queue.push(std::make_pair(dist, node->idx));

    const int axis = node->axis;
    const int dir = position[axis] < train->position[axis] ? 0 : 1;
    k_nearest_neighbor_search_recursive(position, node->next[dir], queue, k);

    const auto diff = (float)fabs(position[axis] - train->position[axis]);
    if ((int) queue.size() < k || diff < queue.back().first)
        k_nearest_neighbor_search_recursive(position, node->next[!dir], queue, k);
}

void PhotonMap::neighbors_in_radius_search_recursive(point position,
                                                                  const PhotonMap::NodePhotonMap *node,
                                                                  std::vector<int> &indices, float radius) const {
    if (node == nullptr)
        return;

    std::shared_ptr<Photon> train = m_photons[node->idx];

    const float dist = distance(position, train);
    if (dist < radius)
        indices.push_back(node->idx);

    const int axis = node->axis;
    const int dir = position[axis] < train->position[axis] ? 0 : 1;
    neighbors_in_radius_search_recursive(position, node->next[dir], indices, radius);

    const auto diff = (float)fabs(position[axis] - train->position[axis]);
    if (diff < radius)
        neighbors_in_radius_search_recursive(position, node->next[!dir], indices, radius);
}

std::shared_ptr<Photon> PhotonMap::get(int i) {
    return m_photons[i];
}

PhotonMap::NodePhotonMap::NodePhotonMap() : idx(-1), axis(-1) { next[0] = next[1] = nullptr; }
