//
// Created by david on 11/18/17.
//

#ifndef GRAPHDATA_LINKGRAPH_H
#define GRAPHDATA_LINKGRAPH_H

#include <graphData.h>

/**
 * @brief Initialize a linked-list graph backing structure
 *
 * Perform a basic initialization of the graph structure of the given type.
 *
 * For the LINKED implementation, little interior work is actually done in this release.  Graph structures of this type
 * are the most general.
 *
 * @param g Graph structure
 * @return GRAPH_OK when g is non-NULL; GRAPH_ERR when g is NULL
 */
int linkGraphInit(struct graph_t *g);


/**
 * @brief Clear out the underlying data structures for the given LINK graph.
 *
 * @param g Graph to be cleared and memory deallocated
 * @return GRAPH_OK, or GRAPH_ERR when destroyNodes or destroyEdges fails.
 * An empty linked graph has a NULL node list, and destroyNodes returns
 * GRAPH_ERR for that NULL pointer, so freeing an empty linked graph fails.
 */
int linkGraphFree(struct graph_t *g);

#endif //GRAPHDATA_LINKGRAPH_H
