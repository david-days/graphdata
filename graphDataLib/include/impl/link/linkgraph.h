//
// Created by david on 11/18/17.
//

#ifndef GRAPHDATA_LINKGRAPH_H
#define GRAPHDATA_LINKGRAPH_H

#include "graphData.h"

/**
 * @brief Initialize a linked-list graph backing structure
 *
 * Perform a basic initialization of the graph structure of the given type.
 *
 * For the LINKED implementation, little interior work is actually done in this release.  Graph structures of this type
 * are the most general.
 *
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @return 1 if successful; 0 if there was a problem.
 */
int linkGraphInit(struct graph_t *g);


/**
 * @brief Clear out the underlying data structures for the given LINK graph.
 *
 * @param g Graph to be cleared and memory deallocated
 * @return 1 if the operation as a success; otherwise, 0.
 */
int linkGraphFree(struct graph_t *g);

#endif //GRAPHDATA_LINKGRAPH_H
