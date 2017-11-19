//
// Created by david on 11/18/17.
//

#ifndef GRAPHDATA_HASHGRAPH_H
#define GRAPHDATA_HASHGRAPH_H

#include <graphdata.h>

/**
 * @brief Initialize the graph using the hashtable adjacency list structure.
 *
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure to be operated upon.
 * @returns 1 if successful; 0 of there was a problem.
 */
int hashGraphInit(enum GRAPHTYPE gtype, struct graph_t *g);

/**
 * @brief Intialize a label graph structure using the hashtable adjacency list structure
 *
 *
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure to be initialized
 * @param lblcount number of labels to be used
 * @return 1 if successful; 0 if there was a problem.
 */
int hashGraphLabelInit(enum GRAPHTYPE gtype, struct graph_t *g, size_t lblcount);


#endif //GRAPHDATA_HASHGRAPH_H