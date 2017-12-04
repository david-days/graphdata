//
// Created by david on 11/18/17.
//
#include <graphdata.h>
#include <impl/linkgraph.h>


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
int linkGraphInit(enum GRAPHTYPE gtype, struct graph_t *g) {
    int retval = 0;
    if (g != NULL) {
        g->gtype = gtype;
        g->gimpl = LINKED;
    }
    return retval;
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

