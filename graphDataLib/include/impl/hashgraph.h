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
 * @returns 1 if successful; 0 of there was a problem.
 */
int hashGraphInit(struct graph_t *g);

/**
 * Empty out and free up the memory used by this hash graph structure
 * @param g Graph structure to be freed
 * @return 1 if successful, 0 if there was a problem
 */
int hashGraphFree(struct graph_t *g)

#endif //GRAPHDATA_HASHGRAPH_H
        