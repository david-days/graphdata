//
// Created by david on 11/18/17.
//
#include <graphdata.h>
#include <impl/linkgraph.h>


/**
 * @brief Initialize a linked-list graph backing structure
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @return 1 if successful; 0 if there was a problem.
 */
int linkGraphInit(enum GRAPHTYPE gtype, struct graph_t *g) {
    return 0;
}

/**
 * @brief Initialize a linked-list graph structure with the given number of avaiable label values
 *
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @param lblcount number of labels to be allocated
 * @return 1 if successful; 0 if there was a problem
 */
int linkGraphLabelInit(enum GRAPHTYPE gtype, struct graph_t *g, size_t lblcount) {
    return 0;
}

