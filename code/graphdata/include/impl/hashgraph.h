//
// Created by david on 11/18/17.
//

#ifndef GRAPHDATA_HASHGRAPH_H
#define GRAPHDATA_HASHGRAPH_H

#include <graphdata.h>

/**
 * @brief Initialize the graph using the hashtable adjacency list structure.
 *
 * @param g Graph structure to be operated upon.
 * @returns 1 if successful; 0 of there was a problem.
 */
int hashGraphInit(struct graph_t *g);


#endif //GRAPHDATA_HASHGRAPH_H
