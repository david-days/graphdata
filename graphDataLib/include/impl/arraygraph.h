/**
 * @brief Array implementation for a fixed node index and a fixed degree.
 *
 * Node ids are 0 .. nodelen-1. Degree is dimensions_t.dimcount.
 * Undirected edges are stored as min(u,v) -> max(u,v). Directed edges are
 * stored as given. An unused adjacency slot is GRAPH_SLOT_EMPTY, not 0.
 */

#ifndef GRAPHDATA_ARRAYGRAPH_H
#define GRAPHDATA_ARRAYGRAPH_H

#include <graphData.h>

/**
 * @brief Metadata structure for array graphs
 */
struct arraydata_t {
    /**
     * @brief Length of the node array
     */
    size_t nodelen;
    /**
     * @brief Length of the edge array
     */
    size_t edgelen;
    /**
     * @brief Degree of the nodes--the number of (possible) edges coming out of each node.
     */
    size_t degree;

    /**
     * @brief Length of the arrays
     *
     * This length is actually nodecount * degree for the ARRAY implementation
     */
    size_t arraylen;
};

/**
 * @brief Set up a graph with array backing data
 *
 * @param g Graph structure
 * @return GRAPH_OK if successful; GRAPH_ERR if an error
 */
int arrayGraphInit(struct graph_t *g);


/**
 * @brief Perform clearing operations to deallocate the array graph internal values and structures.
 * @param g graph_t with array structures to be deallocated
 * @return GRAPH_OK if successful; otherwise, GRAPH_ERR.
 */
int arrayGraphFree(struct graph_t *g);

#endif //GRAPHDATA_ARRAYGRAPH_H
