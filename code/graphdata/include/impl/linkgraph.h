//
// Created by david on 11/18/17.
//

#ifndef GRAPHDATA_LINKGRAPH_H
#define GRAPHDATA_LINKGRAPH_H

#include <graphdata.h>

/**
 * @brief Initialize a linked-list graph backing structure
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @return 1 if successful; 0 if there was a problem.
 */
int linkGraphInit(enum GRAPHTYPE gtype, struct graph_t *g);

/**
 * @brief Initialize a linked-list graph structure with the given number of avaiable label values
 *
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @param lblcount number of labels to be allocated
 * @return 1 if successful; 0 if there was a problem
 */
int linkGraphLabelInit(enum GRAPHTYPE gtype, struct graph_t *g, size_t lblcount);

#endif //GRAPHDATA_LINKGRAPH_H
