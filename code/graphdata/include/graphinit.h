//
// Created by david on 11/11/17.
//

#ifndef GRAPHDATA_GRAPHINIT_H
#define GRAPHDATA_GRAPHINIT_H

#include <graphdata.h>
#include <graphops.h>


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
 * @brief Create and fill the graphops_t structure that handles basic operations for the graph
 *
 * Using the graph_t implementation pass, this function creates a graphops_t structure necessary to handle basic functions
 * for the given graph.
 *
 * @param g Graph structure being used
 * @return Pointer to a new graphops_t structure, if successful; otherwise, a pointer to NULL
 *
 */
struct graphops_t * getOperations(struct graph_t *g);



#endif //GRAPHDATA_GRAPHINIT_H
