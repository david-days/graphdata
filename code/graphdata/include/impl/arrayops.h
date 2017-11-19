//
// Created by david on 11/18/17.
//

#ifndef GRAPHDATA_ARRAYOPS_H
#define GRAPHDATA_ARRAYOPS_H

#include <graphdata.h>

//Read functions to extract data
/**
 * @brief Implementation for getting the node count;
 * @param g Graph structure in question
 * @return Count of nodes, if graph is not null; otherwise, return 0
 */
size_t arrayNodeCount(struct graph_t *g);
/**
 * @brief Implementation to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
size_t arrayEdgeCount(struct graph_t *g);
/**
 * @brief Implementation to retrieve a node structure reference.
 *
 * Implementation-specific on whether this
 * structure is part of the original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, pointer to NULL
 */
struct node_t * arrayGetNode(const size_t *nodeid, const struct graph_t *g);
/**
 * @brief Implementation to retrieve a edge structure reference.
 *
 * Implementation-specific on whether this structure is part of the
 * original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param u nodeid of the starting edge.
 * @param v nodeid of the ending edge.
 * @param g Graph structure in question
 * @return pointer to the edge structure, if found; otherwise, pointer to NULL.
 */
struct edge_t * arrayGetEdge(const size_t *u, const size_t *v, const struct graph_t *g);
/**
 * @brief Implementation to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of node references, starting with the given node, if found; otherwise, pointer to NULL.
 */
struct node_t * arrayGetNeighbors(const size_t *nodeid, const struct graph_t *g);
/**
 * @brief Implementation to retrieve linked-list of edges from a given node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of edges starting from the given node, if found; otherwise, pointer to NULL.
 */
struct edge_t * arrayGetEdges(const size_t *nodeid, const struct graph_t *g);
/**
 * @brief Implementation to retrieve linked-list path of nodes from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of nodes, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
struct node_t * arrayGetNodePath(const size_t *uid, const size_t *vid, const struct graph_t *g);
/**
 * @brief Implementation to retrieve linked-list of path edges from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of edges, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
struct edge_t * arrayGetEdgePath(const size_t *uid, const size_t *vid, const struct graph_t *g);

//Write functions to modify graph
/**
 * @brief Implementation to add a node to a given graph.
 * Not all implementations may use this (for example, fixed-size graphs such as spatial ARRAY implementations).
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return 0 if there was an error, 1 if the node was successfully added
 */
int arrayAddNode(const size_t *nodeid, struct graph_t *g);
/**
 * @brief Implementation to add an edge to a given graph.
 * Not all implementations may use this (for example, fixed-size ARRAY implementations representing a set domain of nodes and relationships).
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return 0 if there was an error; 1 if the edge was successfully added.
 */
int arrayAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g);
/**
 * @brief Implementation to set the capacity (cost, weight, etc.) of an edge in the given graph.
 * @param uid identifier of the edge start
 * @param vid identifier of the edge ending.
 * @param cap capacity value to be set
 * @param g Graph structure in question
 * @return 0 if there was an error; 1 if the capacity was successfully set
 */
int arraySetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g);

/**
 * @brief Implementation to "reset" the graph according to the given argument pointer.
 *
 * For some implementations, it is more efficient to reuse the existing graph structure and perform a "zero-out"
 * of the data, rather than rebuilding from scratch.  This function pointer provides that option.
 * @param g Graph structure to be zeroed or modified according to reset logic
 * @param args Arguments to be used in the reset process, if necessary
 * @param callback Callback to be executed when graph has been reset.
 * @return 0 if there was an error during the reset; 1 if the reset completed;
 */
int arrayResetGraph(struct graph_t *g, void *args, void (*callback)(void));

/**
 * @brief Utility method to free up an array
 *
 * Deallocates all unerlying arrays and the containing array.
 * @param arraylen Number of elements in the array
 * @param arr 2D array to be cleared
 * @return 1 if successful; 0 if error.
 */
int freeGraphArray(size_t arraylen, void** arrptr);


#endif //GRAPHDATA_ARRAYOPS_H
