//
// Created by david on 11/18/17.
//

#ifndef GRAPHDATA_HASHGRAPH_H
#define GRAPHDATA_HASHGRAPH_H

#include <graphData.h>

/**
 * @brief Initialize the graph using the hashtable adjacency list structure.
 *
 * @param g Graph structure to be operated upon.
 * @returns EXIT_SUCCESS if successful; EXIT_FAILURE of there was a problem.
 */
int hashGraphInit(struct graph_t *g);


#endif //GRAPHDATA_HASHGRAPH_H
