#pragma once

#include <iostream>
#include <vector>
#include <assert.h>
#include "Matrix.h"

struct AdjList
{
    std::vector<std::vector<int>> _data;

    AdjList() {}

    AdjList(int V)
        : _data(std::vector<std::vector<int>>(V)) {}

    AdjList(const AdjList &other)
        : _data(other._data) {}

    AdjList(AdjList &&other) noexcept
        : _data(std::move(other._data)) {}

    AdjList &operator=(const AdjList &other)
    {
        if (this != &other)
        {
            _data = other._data;
        }
        return *this;
    }

    AdjList &operator=(AdjList &&other) noexcept
    {
        if (this != &other)
        {
            _data = std::move(other._data);
        }
        return *this;
    }

    std::vector<int> &operator[](int idx)
    {
        return _data[idx];
    }

    const std::vector<int> &operator[](int idx) const
    {
        return _data[idx];
    }

    size_t size() const noexcept
    {
        return _data.size();
    }

    const int edge_size() const noexcept
    {
        int sz = 0;
        for (const auto &vec : _data)
            sz += vec.size();
        return sz;
    }

    void add_edge(int u, int v)
    {
        _data[u].push_back(v);
        _data[v].push_back(u);
    }

    void merge_graphs_from(const AdjList &from)
    {
        assert(from.size() == _data.size());

        int n = from.size();
        for (int u = 0; u < n; ++u)
            _data[u].insert(_data[u].end(), from[u].begin(), from[u].end());
    }

    // Returns edges incident to a vertex of degree < `threshold`.
    AdjList get_edge_subset(int threshold)
    {
        AdjList new_graph(_data.size());

        for (int u = 0; u < (int)_data.size(); ++u)
        {
            int u_deg = _data[u].size();
            for (int v : _data[u])
            {
                int v_deg = _data[v].size();
                if (u_deg < threshold || v_deg < threshold)
                    new_graph[u].push_back(v);
            }
        }

        return new_graph;
    }


    /**
     * @brief   Returns a list of vertices dominating other vertices of deg >= `threshold`,
     *          and edges connecting dominated vertices and dominating ones.
     *          Size of the dominating set: O((V log V) / `threshold`)
     *          Time complexity: O(V + E)
     * @param   threshold   threshold degree of vertices
     */
    std::pair<std::vector<int>, AdjList> dominate(int threshold)
    {
        // number of dominated neighbors (not including itself)
        std::vector<int> dominating_count(_data.size());

        // bucket queue (key: dominating count, value: node index)
        std::vector<std::vector<int>> bucket(_data.size());

        for (int u = 0; u < (int)_data.size(); ++u)
        {
            for (int v : _data[u])
            {
                int v_deg = _data[v].size();
                if (v_deg >= threshold)
                    ++dominating_count[u];
            }

            if (dominating_count[u] > 0)
                bucket[dominating_count[u]].push_back(u);
        }

        std::vector<bool> is_dominated(_data.size());
        std::vector<int> dominating_set;
        AdjList new_graph(_data.size());

        for (int i = _data.size() - 1; i > 0; --i)
        {
            while (!bucket[i].empty())
            {
                // a vertex currently dominating the largest number of vertices
                int u = bucket[i].back();
                bucket[i].pop_back();
                if (i != dominating_count[u])
                    continue; // stale key

                dominating_set.push_back(u);

                int u_deg = _data[u].size();
                if (u_deg >= threshold && !is_dominated[u])
                {
                    is_dominated[u] = true;

                    // update neighbors of dominating vertex
                    for (int v : _data[u])
                        if (--dominating_count[v] > 0)
                            bucket[dominating_count[v]].push_back(v);
                }

                for (int v : _data[u])
                {
                    int v_deg = _data[v].size();
                    if (v_deg >= threshold && !is_dominated[v])
                    {
                        is_dominated[v] = true;
                        new_graph.add_edge(u, v);

                        // update neighbors of dominated vertex
                        for (int x : _data[v])
                            if (x != u && --dominating_count[x] > 0)
                                bucket[dominating_count[x]].push_back(x);
                    }
                }
            }
        }

        return make_pair(dominating_set, new_graph);
    }

    /**
     * @brief   Runs Dijkstra's algorithm with integer weights.
     *          Time complexity: O(V + E)
     * @param   weight  2D array storing estimated distance
     * @param   s       source node index
     */
    void dijkstra(Matrix<int> &weight, int s)
    {
        int max_dist = 2 * (_data.size() + K);
        std::vector<int> dist(_data.size(), max_dist);

        // bucket queue (key: distance, value: node index)
        std::vector<std::vector<int>> bucket(max_dist);

        dist[s] = 0;
        bucket[0].push_back(s);

        for (int i = 0; i < (int)_data.size() + K; ++i)
        {
            while (!bucket[i].empty())
            {
                int u = bucket[i].back();
                bucket[i].pop_back();
                if (i != dist[u])
                    continue;

                // confirmed to be the shortest distance on this graph
                if (i < weight(s, u))
                {
                    weight(s, u) = i;
                    weight(u, s) = i;
                }

                for (int v : _data[u])
                {
                    if (dist[v] > dist[u] + weight(u, v))
                    {
                        dist[v] = dist[u] + weight(u, v);
                        bucket[dist[v]].push_back(v);
                    }
                }
            }
        }
    }
};
