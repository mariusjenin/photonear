//
// Created by mariusjenin on 12/12/22.
//

#ifndef PHOTONEAR_PHOTONMAP_H
#define PHOTONEAR_PHOTONMAP_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <exception>
#include <functional>
#include <cmath>
#include <memory>

#include "glm/glm.hpp"

#include "Photon.h"

namespace ray_tracing{
    class PhotonMap {
    public:
        /** @brief The constructors.
        */
        PhotonMap();

        explicit PhotonMap(const std::vector<std::shared_ptr<Photon>>& photons);

        /** @brief The destructor.
        */
        ~PhotonMap();

        /** @brief Re-builds k-d tree.
        */
        void build(const std::vector<std::shared_ptr<Photon>>& photons);

        /** @brief Clears k-d tree.
        */
        void clear();

        /** @brief Validates k-d tree.
        */
        bool validate() const;

        /** @brief Searches the nearest neighbor.
        */
        int nearest_neighbor_search(point position, float *minDist = nullptr) const;

        /** @brief Searches k-nearest neighbors.
        */
        std::vector<int> k_nearest_neighbors_search(point position, int k) const;

        /** @brief Searches neighbors within radius.
        */
        std::vector<int> neighbors_in_radius_search(point position, float radius) const;

        std::shared_ptr<Photon> get(int i);

    private:

        /** @brief k-d tree node.
        */
        struct NodePhotonMap {
            int idx;       //!< index to the original point
            NodePhotonMap *next[2]{}; //!< pointers to the child nodes
            int axis;      //!< dimension's axis

            NodePhotonMap();
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
        using KNNQueue = BoundedPriorityQueue<std::pair<float, int>>;

        /** @brief Builds k-d tree recursively.
        */
        NodePhotonMap *build_recursive(int *indices, int n_photons, int depth);

        /** @brief Clears k-d tree recursively.
        */
        void clear_recursive(NodePhotonMap *node);

        /** @brief Validates k-d tree recursively.
        */
        void validate_recursive(const NodePhotonMap *node, int depth) const;

        static float distance(const std::shared_ptr<Photon>& p, const std::shared_ptr<Photon>& q);

        static float distance(point position, const std::shared_ptr<Photon>& q);

        /** @brief Searches the nearest neighbor recursively.
        */
        void nearest_neighbor_search_recursive(point position, const NodePhotonMap *node, int *guess, float *minDist) const;

        /** @brief Searches k-nearest neighbors recursively.
        */
        void k_nearest_neighbor_search_recursive(point position, const NodePhotonMap *node, KNNQueue &queue, int k) const;

        /** @brief Searches neighbors within radius.
        */
        void neighbors_in_radius_search_recursive(point position, const NodePhotonMap *node, std::vector<int> &indices, float radius) const;

        NodePhotonMap * m_root;
        std::vector<std::shared_ptr<Photon>> m_photons;
    };
}


#endif //PHOTONEAR_PHOTONMAP_H
