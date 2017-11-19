//
// Created by david on 11/11/17.
//

#ifndef GRAPHDATA_GRAPHINIT_H
#define GRAPHDATA_GRAPHINIT_H

#include <graphdata.h>
#include <graphops.h>

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
 * @brief Initialize a general graph structure.
 *
 * Create a generic graph structure, based on the parameters passed.  The underlying structure will be created in accordance
 * with the combinations of GRAPHTYPE and GRAPHIMPL values.  For GRAPHIMPL = ARRAY, the dimensions value is required and must not be NULL.
 * For other implementations, passing a valid dimensions_t structure is optional (usage dependent).
 *
 * The returned graph structure pointer will  have a NULL pointer for g->labels.
 *
 * @param gtype Type of graph (DIRECTED or UNDIRECTED).
 * @param impltype Type of implementation requested.  Use GENERIC for a general graph.
 * @param dims Dimensions for a spatial graph.  Required for ARRAY implementations, NULL or optional for all others.
 * @return Pointer to the graph structure created, if paramaters are valid.  NULL pointer returned if the parameters are invalid or there is
 * a problem allocating the memory required.
 */
struct graph_t * initGraph(enum GRAPHTYPE gtype, enum GRAPHIMPL impltype, struct dimensions_t *dims);

/**
 * @brief Initialize a graph structure primed for usage in graph-label operations.
 *
 * Creates a graph strucutre, based on the parameters passed, with the g->labels structure filled accordingly.  For GRAPHIMPL = ARRAY, the
 * dimensions value will be created, and the label structure will contain the values for the defined label nodes.  For all others, the
 * label array will be initialized with 0, which may or may not be valid identifiers.
 *
 * @param gtype Type of graph (DIRECTED or UNDIRECTED)
 * @param impltype Type of implementation required.  use Generic for general graph.
 * @param lblcount Number of label values to be used.
 * @param dims Dimensions for spatial graph.  Required for ARRAY implementations, NULL or optional for all others.
 * @return Pointer to the graph structure created, if the parameters are valid.  NULL pointer returned if the parameters are invalid
 * or there is a problem allocating the memory required.
 */
struct graph_t * initLabelGraph(enum GRAPHTYPE gtype, enum GRAPHIMPL impltype, size_t lblcount, struct dimensions_t *dims);

/**
 * @brief Clear the graph and all underlying structures
 *
 * The pointer itself will be changed to NULL
 *
 * @param g Graph structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyGraph(struct graph_t *g);

/**
 * @brief Clear out the dimensions and all underlying structures
 * @param dims Dimensions structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyDimensions(struct dimensions_t *dims);

/**
 * @brief Clear out the label and all underlying structures
 * @param labels Label structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyLabels(struct labels_t *labels);

/**
 * @brief Clear an edge structure and any linked edges (use on single or a path)
 *
 * The pointer itself will be changed to NULL
 *
 * @param e Initial edge pointer
 * @return 1 if success; 0 if error.
 */
int destroyEdges(struct edge_t *e);

/**
 * @brief Clear a graphops_t structure.
 * The graph itself will not be cleared, only the reference to it.  The pointer itself will be changed to NULL
 *
 * @param gops Graphops structuure to be cleared and deallocated
 * @return 1 if successful; 0 if error
 */
int destroyGraphops(struct graphops_t *gops);

/**
 * @brief Clear a node or node list (use on single or a path)
 *
 * The pointer itself will be changed to NULL
 *
 * @param n Initial node pointer
 * @return 1 if successful; 0 if error
 */
int destroyNodes(struct node_t *n);

/**
 * @brief Clear a feature or feature list (use on single or multiple attributes)
 *
 * The pointer itself will be changed to NULL
 *
 * @param f Initial feature pointer
 * @return 1 if successful; 0 if error
 */
int destroyFeatures(struct feature_t *f);

#endif //GRAPHDATA_GRAPHINIT_H
