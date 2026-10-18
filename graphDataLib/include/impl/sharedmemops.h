//
// Created by david on 10/21/23.
//

#ifndef GRAPHDATA_SHAREDMEMOPS_H
#define GRAPHDATA_SHAREDMEMOPS_H

#include <graphData.h>

//Read functions to extract data
/**
 * @brief Implementation for getting the node count;
 * @param g Graph structure in question
 * @return Count of nodes, if graph is not null; otherwise, return 0
 */
size_t shmemNodeCount(struct graph_t *g);
/**
 * @brief Implementation to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
size_t shmemEdgeCount(struct graph_t *g);
/**
 * @brief Implementation to retrieve a node structure reference.
 *
 * Caller-owned copy (nodeid only). Release with destroyNodes.
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, NULL
 */
struct node_t * shmemGetNode(const size_t *nodeid, const struct graph_t *g);
/**
 * @brief Remove a node from the graph.
 *
 * NOOP implementation.  ARRAY-based graphs are defined at initialization and do not allow removing nodes.
 *
 * @param nodeid Node id to be removed.
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemRemoveNode(const size_t *nodeid, struct graph_t *g);
/**
 * @brief Implementation to retrieve a edge structure reference.
 *
 * Caller-owned copy (u, v, cap, and flow). Release with destroyEdges.
 * @param u nodeid of the starting edge.
 * @param v nodeid of the ending edge.
 * @param g Graph structure in question
 * @return pointer to the edge structure, if found; otherwise, NULL.
 */
struct edge_t * shmemGetEdge(const size_t *u, const size_t *v, const struct graph_t *g);
/**
 * @brief Implementation to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Caller-owned list of neighbor ids. The queried node is not included.
 * Release the chain with destroyNodes. NULL when the node is absent or has no neighbors.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return caller-owned neighbor chain, or NULL
 */
struct node_t * shmemGetNeighbors(const size_t *nodeid, const struct graph_t *g);
/**
 * @brief Implementation to retrieve linked-list of edges from a given node.
 * Caller-owned copies of the node's edges. Release the chain with destroyEdges.
 * NULL when the node is absent or has no edges.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return caller-owned edge chain, or NULL
 */
struct edge_t * shmemGetEdges(const size_t *nodeid, const struct graph_t *g);
/**
 * @brief Function pointer to retrieve the current capacity value for a given edge.
 *
 * For implementations that support this, the capacity value will be written to the *cap parameter.
 *
 * @param uid Edge start identifier
 * @param vid Edge end identifier
 * @param cap Capacity value pointer to store the value
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g);
/**
 * @brief Function pointer to retrieve the current flow value for a given edge.
 *
 * For implementations that support this, the flow value will be written to the *flow parameter.
 *
 * @param uid Edge start identifier
 * @param vid Edge end identifier
 * @param flow Flow value pointer to store the result
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g);
//Write functions to modify graph
/**
 * @brief Implementation to add a node to a given graph.
 * Not all implementations may use this (for example, fixed-size graphs such as spatial ARRAY implementations).
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemAddNode(const size_t *nodeid, struct graph_t *g);
/**
 * @brief Implementation to add an edge to a given graph.
 * Returns GRAPH_ERR when the graph is read-only or every adjacency slot is full.
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g);
/**
 * @brief Function pointer to remove an edge from the given graph.
 *
 * Remove references for an edge within the structure.
 *
 * @param uid Identifier for the edge start
 * @param vid Identifier for the edge end.
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g);
/**
 * @brief Implementation to set the capacity (cost, weight, etc.) of an edge in the given graph.
 * @param uid identifier of the edge start
 * @param vid identifier of the edge ending.
 * @param cap capacity value to be set
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemSetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g);
/**
 * @brief Function to add (adjust) the capacity for an edge by a given amount.
 *
 * Add the capacity value to the current capacity for the given edge.
 *
 * @param uid Identifier of the edge start
 * @param vid Identifier of the edge end.
 * @param cap Value to adjust the capacity
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g);
/**
 * @brief Function to set the flow value for an edge (amount of capacity currently "used")
 *
 * Set the flow value for the given edge.
 *
 * @param uid Identifier of the edge start.
 * @param vid Identifier of the edge end.
 * @param flow Value to be set for the flow.
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemSetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g);
/**
 * @brief Function to adjust the flow value of a given edge.
 *
 * Add the flow value to the flow tracking of the given graph.
 *
 * @param uid Identifier of the edge start
 * @param vid Identifier of the edge end.
 * @param flow The value to be added to adjust the flow value.
 * @param g The graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g);
/**
 * @brief Implementation to "reset" the graph according to the given argument pointer.
 *
 * For some implementations, it is more efficient to reuse the existing graph structure and perform a "zero-out"
 * of the data, rather than rebuilding from scratch.  This function pointer provides that option.
 * @param g Graph structure to be zeroed or modified according to reset logic
 * @param args Arguments to be used in the reset process, if necessary
 * @param callback Callback to be executed when graph has been reset.
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int shmemResetGraph(struct graph_t *g, void *args, void (*callback)(void));

struct node_t * shmemListNodes(const struct graph_t *g);

int shmemForeachNode(const struct graph_t *g, int (*visit)(const size_t *nodeid, void *ctx), void *ctx);


#endif //GRAPHDATA_SHAREDMEMOPS_H
