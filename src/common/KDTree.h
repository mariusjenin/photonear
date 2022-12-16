//
// Created by mariusjenin on 15/12/22.
//

#ifndef PHOTONEAR_KDTREE_H
#define PHOTONEAR_KDTREE_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <exception>
#include <functional>
#include <cmath>

#include "glm/glm.hpp"

#include "Photon.h"

using namespace ray_tracing;

/** @brief k-d tree class.
*/
class KDTree {
public:
    /** @brief The constructors.
    */
    KDTree() : m_root(nullptr) {};

    explicit KDTree(const std::vector<Photon> &photons) : m_root(nullptr) { build(photons); }

    /** @brief The destructor.
    */
    ~KDTree() { clear(); }

    /** @brief Re-builds k-d tree.
    */
    void build(const std::vector<Photon> &photons) {
        clear();

        m_photons = photons;

        std::vector<int> indices(photons.size());
        std::iota(std::begin(indices), std::end(indices), 0);

        m_root = build_recursive(indices.data(), (int) photons.size(), 0);
    }

    /** @brief Clears k-d tree.
    */
    void clear() {
        clear_recursive(m_root);
        m_root = nullptr;
        m_photons.clear();
    }

    /** @brief Validates k-d tree.
    */
    bool validate() const {
        try {
            validate_recursive(m_root, 0);
        }
        catch (const std::exception &) {
            return false;
        }

        return true;
    }

    /** @brief Searches the nearest neighbor.
    */
    int nearest_neighbor_search(const Photon &query, double *minDist = nullptr) const {
        int guess;
        double _minDist = std::numeric_limits<double>::max();

        nearest_neighbor_search_recursive(query, m_root, &guess, &_minDist);

        if (minDist)
            *minDist = _minDist;

        return guess;
    }

    /** @brief Searches k-nearest neighbors.
    */
    std::vector<int> k_nearest_neighbors_search(const Photon &query, int k) const {
        KNNQueue queue(k);
        k_nearest_neighbor_search_recursive(query, m_root, queue, k);

        std::vector<int> indices(queue.size());
        for (size_t i = 0; i < queue.size(); i++)
            indices[i] = queue[i].second;

        return indices;
    }

    /** @brief Searches neighbors within radius.
    */
    std::vector<int> neighbors_in_radius_search(const Photon &query, double radius) const {
        std::vector<int> indices;
        neighbors_in_radius_search_recursive(query, m_root, indices, radius);
        return indices;
    }

private:

    /** @brief k-d tree node.
    */
    struct NodeKDTree {
        int idx;       //!< index to the original point
        NodeKDTree *next[2]{}; //!< pointers to the child nodes
        int axis;      //!< dimension's axis

        NodeKDTree() : idx(-1), axis(-1) { next[0] = next[1] = nullptr; }
    };

    /** @brief Bounded priority queue.
    */
    template<class T, class Compare = std::less<T>>
    class BoundedPriorityQueue {
    public:

        BoundedPriorityQueue() = delete;

        explicit BoundedPriorityQueue(size_t bound) : m_bound(bound) { m_elements.reserve(bound + 1); };

        void push(const T &val) {
            auto it = std::find_if(std::begin(m_elements), std::end(m_elements),
                                   [&](const T &element) { return Compare()(val, element); });
            m_elements.insert(it, val);

            if (m_elements.size() > m_bound)
                m_elements.resize(m_bound);
        }

        const T &back() const { return m_elements.back(); };

        const T &operator[](size_t index) const { return m_elements[index]; }

        size_t size() const { return m_elements.size(); }

    private:
        size_t m_bound;
        std::vector<T> m_elements;
    };

    /** @brief Priority queue of <distance, index> pair.
    */
    using KNNQueue = BoundedPriorityQueue<std::pair<double, int>>;

    /** @brief Builds k-d tree recursively.
    */
    NodeKDTree *build_recursive(int *indices, int n_photons, int depth) {
        if (n_photons <= 0)
            return nullptr;

        const int axis = depth % 3;
        const int mid = (n_photons - 1) / 2;

        std::nth_element(indices, indices + mid, indices + n_photons, [&](int lhs, int rhs) {
            return m_photons[lhs].position[axis] < m_photons[rhs].position[axis];
        });

        auto *node = new NodeKDTree();
        node->idx = indices[mid];
        node->axis = axis;

        node->next[0] = build_recursive(indices, mid, depth + 1);
        node->next[1] = build_recursive(indices + mid + 1, n_photons - mid - 1, depth + 1);

        return node;
    }

    /** @brief Clears k-d tree recursively.
    */
    void clear_recursive(NodeKDTree *node) {
        if (node == nullptr)
            return;

        if (node->next[0])
            clear_recursive(node->next[0]);

        if (node->next[1])
            clear_recursive(node->next[1]);

        delete node;
    }

    /** @brief Validates k-d tree recursively.
    */
    void validate_recursive(const NodeKDTree *node, int depth) const {
        if (node == nullptr)
            return;

        const int axis = node->axis;
        const NodeKDTree *node0 = node->next[0];
        const NodeKDTree *node1 = node->next[1];

        if (node0 && node1) {
            if (m_photons[node->idx].position[axis] < m_photons[node0->idx].position[axis])
                throw std::exception();

            if (m_photons[node->idx].position[axis] > m_photons[node1->idx].position[axis])
                throw std::exception();
        }

        if (node0)
            validate_recursive(node0, depth + 1);

        if (node1)
            validate_recursive(node1, depth + 1);
    }

    static double distance(const Photon &p, const Photon &q) {
        double dist = 0;
        for (int i = 0; i < 3; i++)
            dist += (p.position[i] - q.position[i]) * (p.position[i] - q.position[i]);
        return sqrt(dist);
    }

    /** @brief Searches the nearest neighbor recursively.
    */
    void nearest_neighbor_search_recursive(const Photon &query, const NodeKDTree *node, int *guess, double *minDist) const {
        if (node == nullptr)
            return;

        const Photon &train = m_photons[node->idx];

        const double dist = distance(query, train);
        if (dist < *minDist) {
            *minDist = dist;
            *guess = node->idx;
        }

        const int axis = node->axis;
        const int dir = query.position[axis] < train.position[axis] ? 0 : 1;
        nearest_neighbor_search_recursive(query, node->next[dir], guess, minDist);

        const double diff = fabs(query.position[axis] - train.position[axis]);
        if (diff < *minDist)
            nearest_neighbor_search_recursive(query, node->next[!dir], guess, minDist);
    }

    /** @brief Searches k-nearest neighbors recursively.
    */
    void k_nearest_neighbor_search_recursive(const Photon &query, const NodeKDTree *node, KNNQueue &queue, int k) const {
        if (node == nullptr)
            return;

        const Photon &train = m_photons[node->idx];

        const double dist = distance(query, train);
        queue.push(std::make_pair(dist, node->idx));

        const int axis = node->axis;
        const int dir = query.position[axis] < train.position[axis] ? 0 : 1;
        k_nearest_neighbor_search_recursive(query, node->next[dir], queue, k);

        const double diff = fabs(query.position[axis] - train.position[axis]);
        if ((int) queue.size() < k || diff < queue.back().first)
            k_nearest_neighbor_search_recursive(query, node->next[!dir], queue, k);
    }

    /** @brief Searches neighbors within radius.
    */
    void neighbors_in_radius_search_recursive(const Photon &query, const NodeKDTree *node, std::vector<int> &indices, double radius) const {
        if (node == nullptr)
            return;

        const Photon &train = m_photons[node->idx];

        const double dist = distance(query, train);
        if (dist < radius)
            indices.push_back(node->idx);

        const int axis = node->axis;
        const int dir = query.position[axis] < train.position[axis] ? 0 : 1;
        neighbors_in_radius_search_recursive(query, node->next[dir], indices, radius);

        const double diff = fabs(query.position[axis] - train.position[axis]);
        if (diff < radius)
            neighbors_in_radius_search_recursive(query, node->next[!dir], indices, radius);
    }

    NodeKDTree * m_root;
    std::vector<Photon> m_photons;
};

#endif //PHOTONEAR_KDTREE_H
