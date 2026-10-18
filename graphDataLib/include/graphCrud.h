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
 * @brief Create a dimension structure containing the given values in order (x, y, z, etc)
 *
 * This is used to initialize (and contain) the ordered size of the spatial dimensions given.  Consumers are responsible
 * for calling destroyDimensions() on the structure when finished.
 *
 * @param dimval First dimension in the ordered list of dimension limits
 * @param ... Additional dimension values passed
 * @return Pointer to a dimensions_t structure that holds the count and values of the structure
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
 * @return pointer to the cloned structure, if successful; otherwise, a pointer to NULL
 */
struct node_t * cloneNode(const struct node_t *onode);

/**
 * @brief Create a copy of the given edge
 *
 * This operation is intended to be used for extracting data from the graph to be used for external purposes, such as
 * processing stacks, queues, and paths, without unintentionally altering the underlying graph structure.
 *
 * @param oedge Original edge_t structure to be cloned.
 * @return pointer to the cloned data, if successful; otherwise, a pointer to NULL
 */
struct edge_t * cloneEdge(const struct edge_t *oedge);

/**
 * @brief Create a copy of the given feature data
 *
 * @param ofeat Original feature to be copied.
 * @return pointer to the cloned data, if successful; otherwise, a pointer to NULL
 */
struct feature_t * cloneFeature(const struct feature_t *ofeat);

/**
 * @brief Clear the graph_t shell.
 *
 * Backing storage should already have been released with clearGraph. The
 * pointer itself will be changed to NULL. This does not free dims or labels.
 *
 * @param gptr Graph structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyGraph(void** gptr);

/**
 * @brief Clear out the dimensions and all underlying structures
 * @param dimptr Dimensions structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyDimensions(void** dimptr);

/**
 * @brief Clear out the label and all underlying structures
 * @param lblptr Label structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyLabels(void** lblptr);

/**
 * @brief Clear an edge structure and any linked edges (use on single or a path)
 *
 * The pointer itself will be changed to NULL
 *
 * @param eptr Initial edge pointer
 * @return 1 if success; 0 if error.
 */
int destroyEdges(void** eptr);

/**
 * @brief Clear a graphops_t structure.
 * The graph itself will not be cleared, only the reference to it.  The pointer itself will be changed to NULL
 *
 * @param optptr Graphops structuure to be cleared and deallocated
 * @return EXIT_SUCCESS if successful; EXIT_FAILURE if error
 */
int destroyGraphops(void** opsptr);

/**
 * @brief Clear a node or node list (use on single or a path)
 *
 * The pointer itself will be changed to NULL. Use this to release getNode,
 * getNeighbors, and listNodes results.
 *
 * @param nptr Initial node pointer
 * @return EXIT_SUCCESS if successful; EXIT_FAILURE if error
 */
int destroyNodes(void** nptr);

/**
 * @brief Clear a feature or feature list (use on single or multiple attributes)
 *
 * The pointer itself will be changed to NULL
 *
 * @param fptr Initial feature pointer
 * @return EXIT_SUCCESS if successful; EXIT_FAILURE if error
 */
int destroyFeatures(void** fptr);

#endif //GRAPHDATA_GRAPHCRUD_H
