/**
 * @file graph.c
 * @brief Implementation of Adjacency List Graph with BFS & DFS in C11.
 */

#include "graph.h"
#include <stdlib.h>

static GraphAdjNode *adj_node_create(size_t dest, int weight) {
    GraphAdjNode *node = malloc(sizeof(GraphAdjNode));
    if (!node) return NULL;
    node->dest = dest;
    node->weight = weight;
    node->next = NULL;
    return node;
}

ds_status_t graph_init(Graph *g, size_t num_vertices, bool is_directed) {
    if (!g || num_vertices == 0) return DS_ERR_INVALID_ARG;

    g->adj = calloc(num_vertices, sizeof(GraphAdjNode *));
    if (!g->adj) return DS_ERR_OUT_OF_MEMORY;

    g->num_vertices = num_vertices;
    g->is_directed = is_directed;
    return DS_SUCCESS;
}

void graph_destroy(Graph *g) {
    if (!g || !g->adj) return;

    for (size_t i = 0; i < g->num_vertices; i++) {
        GraphAdjNode *curr = g->adj[i];
        while (curr) {
            GraphAdjNode *next = curr->next;
            free(curr);
            curr = next;
        }
    }

    free(g->adj);
    g->adj = NULL;
    g->num_vertices = 0;
}

static ds_status_t graph_add_directed_edge(Graph *g, size_t src, size_t dest, int weight) {
    GraphAdjNode *node = adj_node_create(dest, weight);
    if (!node) return DS_ERR_OUT_OF_MEMORY;

    node->next = g->adj[src];
    g->adj[src] = node;
    return DS_SUCCESS;
}

ds_status_t graph_add_edge(Graph *g, size_t src, size_t dest, int weight) {
    if (!g || !g->adj) return DS_ERR_NULL_PTR;
    if (src >= g->num_vertices || dest >= g->num_vertices) return DS_ERR_OUT_OF_BOUNDS;

    ds_status_t status = graph_add_directed_edge(g, src, dest, weight);
    if (status != DS_SUCCESS) return status;

    if (!g->is_directed && src != dest) {
        status = graph_add_directed_edge(g, dest, src, weight);
    }
    return status;
}

bool graph_has_edge(const Graph *g, size_t src, size_t dest) {
    if (!g || src >= g->num_vertices || dest >= g->num_vertices) return false;

    GraphAdjNode *curr = g->adj[src];
    while (curr) {
        if (curr->dest == dest) return true;
        curr = curr->next;
    }
    return false;
}

ds_status_t graph_bfs(const Graph *g, size_t start_vertex, void (*visit_cb)(size_t v)) {
    if (!g || !visit_cb) return DS_ERR_NULL_PTR;
    if (start_vertex >= g->num_vertices) return DS_ERR_OUT_OF_BOUNDS;

    bool *visited = calloc(g->num_vertices, sizeof(bool));
    size_t *queue = malloc(g->num_vertices * sizeof(size_t));
    if (!visited || !queue) {
        free(visited);
        free(queue);
        return DS_ERR_OUT_OF_MEMORY;
    }

    size_t head = 0;
    size_t tail = 0;

    visited[start_vertex] = true;
    queue[tail++] = start_vertex;

    while (head < tail) {
        size_t u = queue[head++];
        visit_cb(u);

        GraphAdjNode *curr = g->adj[u];
        while (curr) {
            size_t v = curr->dest;
            if (!visited[v]) {
                visited[v] = true;
                queue[tail++] = v;
            }
            curr = curr->next;
        }
    }

    free(visited);
    free(queue);
    return DS_SUCCESS;
}

static void dfs_recursive(const Graph *g, size_t u, bool *visited, void (*visit_cb)(size_t v)) {
    visited[u] = true;
    visit_cb(u);

    GraphAdjNode *curr = g->adj[u];
    while (curr) {
        size_t v = curr->dest;
        if (!visited[v]) {
            dfs_recursive(g, v, visited, visit_cb);
        }
        curr = curr->next;
    }
}

ds_status_t graph_dfs(const Graph *g, size_t start_vertex, void (*visit_cb)(size_t v)) {
    if (!g || !visit_cb) return DS_ERR_NULL_PTR;
    if (start_vertex >= g->num_vertices) return DS_ERR_OUT_OF_BOUNDS;

    bool *visited = calloc(g->num_vertices, sizeof(bool));
    if (!visited) return DS_ERR_OUT_OF_MEMORY;

    dfs_recursive(g, start_vertex, visited, visit_cb);

    free(visited);
    return DS_SUCCESS;
}

size_t graph_vertex_count(const Graph *g) {
    return g ? g->num_vertices : 0;
}

