//
// Created by david on 11/18/17.
//
#include "graphData.h"
#include "impl/link/linkgraph.h"
#include "util/crudops.h"


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
int linkGraphInit(struct graph_t *g) {
    int retval = 0;
    if (g != NULL) {
        //TODO:  Fill in basic structure
        retval = 1;
    }
    return retval;
}

/**
 * @brief Clear out the underlying data structures for the given LINK graph.
 *
 * @param g Graph to be cleared and memory deallocated
 * @return 1 if the operation as a success; otherwise, 0.
 */
int linkGraphFree(struct graph_t *g) {
    int retval = 1;
    if (g != NULL) {
        if ((g->gtype & LINKED) == LINKED) {
            struct node_t *currnode = (struct node_t *)g->nodeImpl;
            while (currnode != NULL) {
                retval = retval & destroyEdges((void **)&currnode->edges);
                currnode = currnode->next;
            }
            //now clear out nodes
            retval = retval & destroyNodes(&(g->nodeImpl));
        }
    }
    return retval;
}