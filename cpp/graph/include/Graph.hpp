/**
 * @file Graph.hpp
 * @brief Modern C++20 Adjacency List Graph with BFS & DFS.
 */

#ifndef DS_GRAPH_HPP
#define DS_GRAPH_HPP

#include <vector>
#include <queue>
#include <cstddef>
#include <stdexcept>
#include <functional>

namespace ds {

template <typename Weight = int>
class Graph {
public:
    using vertex_t = std::size_t;

    struct Edge {
        vertex_t to;
        Weight weight;
    };

    explicit Graph(vertex_t num_vertices, bool directed = false)
        : adj_(num_vertices), directed_(directed) {}

    ~Graph() = default;
    Graph(const Graph &) = default;
    Graph &operator=(const Graph &) = default;
    Graph(Graph &&) noexcept = default;
    Graph &operator=(Graph &&) noexcept = default;

    /**
     * @brief Adds an edge from src to dest.
     */
    void add_edge(vertex_t src, vertex_t dest, Weight weight = Weight{1}) {
        check_vertex(src);
        check_vertex(dest);
        adj_[src].push_back({dest, weight});
        if (!directed_ && src != dest) {
            adj_[dest].push_back({src, weight});
        }
    }

    /**
     * @brief Checks if edge exists between src and dest.
     */
    [[nodiscard]] bool has_edge(vertex_t src, vertex_t dest) const {
        check_vertex(src);
        check_vertex(dest);
        for (const auto &edge : adj_[src]) {
            if (edge.to == dest) return true;
        }
        return false;
    }

    /**
     * @brief Breadth-First Search from start_vertex, invoking callback on each vertex.
     */
    void bfs(vertex_t start_vertex, const std::function<void(vertex_t)> &callback) const {
        check_vertex(start_vertex);
        std::vector<bool> visited(adj_.size(), false);
        std::queue<vertex_t> q;

        visited[start_vertex] = true;
        q.push(start_vertex);

        while (!q.empty()) {
            vertex_t u = q.front();
            q.pop();
            callback(u);

            for (const auto &edge : adj_[u]) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }
    }

    /**
     * @brief Depth-First Search from start_vertex, invoking callback on each vertex.
     */
    void dfs(vertex_t start_vertex, const std::function<void(vertex_t)> &callback) const {
        check_vertex(start_vertex);
        std::vector<bool> visited(adj_.size(), false);
        dfs_helper(start_vertex, visited, callback);
    }

    [[nodiscard]] vertex_t vertex_count() const noexcept {
        return adj_.size();
    }

    [[nodiscard]] bool is_directed() const noexcept {
        return directed_;
    }

    [[nodiscard]] const std::vector<Edge> &neighbors(vertex_t v) const {
        check_vertex(v);
        return adj_[v];
    }

private:
    std::vector<std::vector<Edge>> adj_;
    bool directed_;

    void check_vertex(vertex_t v) const {
        if (v >= adj_.size()) {
            throw std::out_of_range("Graph: vertex index out of bounds");
        }
    }

    void dfs_helper(vertex_t u, std::vector<bool> &visited, const std::function<void(vertex_t)> &callback) const {
        visited[u] = true;
        callback(u);

        for (const auto &edge : adj_[u]) {
            if (!visited[edge.to]) {
                dfs_helper(edge.to, visited, callback);
            }
        }
    }
};

} // namespace ds

#endif // DS_GRAPH_HPP
