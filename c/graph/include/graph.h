/**
 * @file graph.h
 * @brief Adjacency List Graph with BFS and DFS in C11.
 */

#ifndef DS_GRAPH_H
#define DS_GRAPH_H

#include "common.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GraphAdjNode {
    size_t dest;                    /**< Destination vertex index */
    int weight;                     /**< Edge weight */
    struct GraphAdjNode *next;      /**< Pointer to next edge in neighbor list */
} GraphAdjNode;

typedef struct {
    GraphAdjNode **adj;             /**< Array of adjacency head pointers */
    size_t num_vertices;            /**< Total vertices [0..num_vertices-1] */
    bool is_directed;               /**< True if directed, false if undirected */
} Graph;

/**
 * @brief Initializes a graph with fixed number of vertices.
 */
ds_status_t graph_init(Graph *g, size_t num_vertices, bool is_directed);

/**
 * @brief Destroys all allocated edge nodes and adjacency lists.
 */
void graph_destroy(Graph *g);

/**
 * @brief Adds an edge from src to dest with optional weight.
 */
ds_status_t graph_add_edge(Graph *g, size_t src, size_t dest, int weight);

/**
 * @brief Checks if an edge exists from src to dest.
 */
bool graph_has_edge(const Graph *g, size_t src, size_t dest);

/**
 * @brief Performs Breadth-First Search (BFS) starting at start_vertex,
 * invoking visit_cb on each visited vertex.
 */
ds_status_t graph_bfs(const Graph *g, size_t start_vertex, void (*visit_cb)(size_t v));

/**
 * @brief Performs Depth-First Search (DFS) starting at start_vertex,
 * invoking visit_cb on each visited vertex.
 */
ds_status_t graph_dfs(const Graph *g, size_t start_vertex, void (*visit_cb)(size_t v));

/**
 * @brief Returns total number of vertices.
 */
size_t graph_vertex_count(const Graph *g);

#ifdef __cplusplus
}
#endif

#endif // DS_GRAPH_H
