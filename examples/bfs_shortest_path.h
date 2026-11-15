/**
 * @brief Unweighted shortest path over the public graphData API.
 *
 * Workspace (visited, distance, parent, queue) stays in the caller. The walk
 * uses listNodes to learn which ids exist and getNeighbors to follow edges.
 */

#ifndef GRAPHDATA_BFS_SHORTEST_PATH_H
#define GRAPHDATA_BFS_SHORTEST_PATH_H

#include <graphInit.h>
#include <graphCrud.h>

/**
 * @brief Find a shortest directed path from source to target.
 *
 * On success, path_out receives the node ids from source through target,
 * and *path_len is the number of ids written. Distance is *path_len - 1.
 *
 * @return GRAPH_OK when a path is found and fits in path_out; GRAPH_ERR otherwise
 */
int bfsShortestPath(struct graphops_t *ops, struct graph_t *g,
                    size_t source, size_t target,
                    size_t *path_out, size_t path_cap, size_t *path_len);

#endif
