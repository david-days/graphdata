/**
 * @brief Caller-owned graph structure create, clone, and destroy operations.
 *
 * These functions are the installed lifecycle API. Callers use them to build
 * dimensions, copy nodes and edges, and release snapshots returned by the
 * public getters and listNodes. Implementation helpers stay in util/crudops.h,
 * which is not installed.
 */

#ifndef GRAPHDATA_GRAPHCRUD_H
#define GRAPHDATA_GRAPHCRUD_H

#include <graphData.h>
#include <graphOps.h>

/**
 * @brief Create a dimension structure for the given sizes.
 *
 * The first argument is the number of dimensions. The remaining arguments are
 * that many sizes, in order. Release the result with destroyDimensions.
 *
 * @param dimval Number of dimensions
 * @param ... One size_t extent per dimension
 * @return New dimensions_t, or NULL if allocation fails
 */
struct dimensions_t * createDimensions(size_t dimval, ...);

/**
 * @brief Allocate and initialize an edge
 * @return pointer to new edge_t memory, if successful; otherwise NULL.
 */
struct edge_t * initEdge();

/**
 * @brief Allocate and initialize a node
 *
 * @return pointer to new node_t memory, if successful; otherwise, NULL.
 */
struct node_t * initNode();

/**
 * @brief Allocate and initialize a feature structure
 * @return pointer to new feature_t memory, if successful; otherwise, NULL.
 */
struct feature_t * initFeature();

/**
 * @brief Create a copy of the given node
 *
 * This operation is intended to be used for extracting data from the graph to be used for external purposes, such as
 * delineating paths, without unintentionally altering the underlying graph structure.
 *
 * @param onode Original node_t to be cloned
 * @return pointer to the cloned structure, if successful; otherwise, NULL
 */
struct node_t * cloneNode(const struct node_t *onode);

/**
 * @brief Create a copy of the given edge
 *
 * This operation is intended to be used for extracting data from the graph to be used for external purposes, such as
 * processing stacks, queues, and paths, without unintentionally altering the underlying graph structure.
 *
 * @param oedge Original edge_t structure to be cloned.
 * @return pointer to the cloned data, if successful; otherwise, NULL
 */
struct edge_t * cloneEdge(const struct edge_t *oedge);

/**
 * @brief Create a copy of the given feature data
 *
 * @param ofeat Original feature to be copied.
 * @return pointer to the cloned data, if successful; otherwise, NULL
 */
struct feature_t * cloneFeature(const struct feature_t *ofeat);

/**
 * @brief Clear the graph_t shell.
 *
 * Backing storage should already have been released with clearGraph. The
 * pointer itself will be changed to NULL. This does not free dims or labels.
 *
 * @param gptr Graph structure to be cleared
 * @return GRAPH_OK when the shell was freed; GRAPH_ERR when *gptr is NULL
 */
int destroyGraph(void** gptr);

/**
 * @brief Clear out the dimensions and all underlying structures
 * @param dimptr Dimensions structure to be cleared
 * @return GRAPH_OK when the structure was freed; GRAPH_ERR when *dimptr is NULL
 */
int destroyDimensions(void** dimptr);

/**
 * @brief Clear out the label and all underlying structures
 * @param lblptr Label structure to be cleared
 * @return GRAPH_OK, including when *lblptr is already NULL
 */
int destroyLabels(void** lblptr);

/**
 * @brief Clear an edge structure and any linked edges (use on single or a path)
 *
 * The pointer itself will be changed to NULL
 *
 * @param eptr Initial edge pointer
 * @return GRAPH_OK when the chain was freed; GRAPH_ERR when *eptr is NULL
 */
int destroyEdges(void** eptr);

/**
 * @brief Clear a graphops_t structure.
 * The graph itself will not be cleared, only the reference to it.  The pointer itself will be changed to NULL
 *
 * @param optptr Graphops structuure to be cleared and deallocated
 * @return GRAPH_OK if successful; GRAPH_ERR if error
 */
int destroyGraphops(void** opsptr);

/**
 * @brief Clear a node or node list (use on single or a path)
 *
 * The pointer itself will be changed to NULL. Use this to release getNode,
 * getNeighbors, and listNodes results.
 *
 * @param nptr Initial node pointer
 * @return GRAPH_OK if successful; GRAPH_ERR if error
 */
int destroyNodes(void** nptr);

/**
 * @brief Clear a feature or feature list (use on single or multiple attributes)
 *
 * The pointer itself will be changed to NULL
 *
 * @param fptr Initial feature pointer
 * @return GRAPH_OK if successful; GRAPH_ERR if error
 */
int destroyFeatures(void** fptr);

#endif //GRAPHDATA_GRAPHCRUD_H
