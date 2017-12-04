/**
 * @brief This header is for basic graph structure operations (Create,Read,Update,Delete and similar).
 *
 * The functions specified here are for general graph structures and data, for use in various other implementations.  These
 * are the most general operations, dealing with basic graph structures (nodes, edges, dimensional data, etc.).  For
 * implementation-specific operations, use the XXXgraph and XXXops headers under the impl folders.
 *
 *
 */

#ifndef GRAPHDATA_CRUDOPS_H
#define GRAPHDATA_CRUDOPS_H

#include <graphdata.h>
#include <graphops.h>
#include <stdlib.h>

//Creation operations

/**
 * @brief Utility method to create and preset graph structure.
 * @return A pointer to a graph_t structure, if successful; otherwise, a pointer to NULL
 */
struct graph_t * basicGraphInit();

/**
 * @brief Create a dimension structure containing the given values in order (x, y, z, etc)
 *
 * This is used to initialize (and contain) the ordered size of the spatial dimensions given.  Consumers are responsible
 * for calling free() on the structure, or passing it to a standard clearing function
 *
 * @param dimval First dimension in the ordered list of dimension limits
 * @param ... Additional dimension values passed
 * @return Pointer to a dimensions_t structure that holds the count and values of the structure
 */
struct dimensions_t * createDimensions(size_t dimval, ...);

/**
 * @brief Create a raw label structure of the given size
 * @param lblcount Number of labels required
 * @return Label structure properly initialized with a size_t array of the given size, if successful; otherwise, a NULL
 * pointer.
 */
struct labels_t * initLabels(size_t lblcount);


/**
 * @brief Raw initializer for graphops_t structure
 *
 * Consumers of this object are responsible for calling free() on the pointer when finished, or passing it to
 * a standard cleanup function.
 *
 * @return Pointer to a graphops structure, or NULL if there was a problem with memory allocation
 */
struct graphops_t * initGraphops();

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

//Clone operations

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


//Free operations.
/**
 * @brief Clear the graph and all underlying structures
 *
 * The pointer itself will be changed to NULL
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
 * @return 1 if successful; 0 if error
 */
int destroyGraphops(void** opsptr);

/**
 * @brief Clear a node or node list (use on single or a path)
 *
 * The pointer itself will be changed to NULL
 *
 * @param nptr Initial node pointer
 * @return 1 if successful; 0 if error
 */
int destroyNodes(void** nptr);

/**
 * @brief Clear a feature or feature list (use on single or multiple attributes)
 *
 * The pointer itself will be changed to NULL
 *
 * @param fptr Initial feature pointer
 * @return 1 if successful; 0 if error
 */
int destroyFeatures(void** fptr);

#endif //GRAPHDATA_CRUDOPS_H
