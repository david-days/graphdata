//
// Created by david on 11/18/17.
//
#include <graphData.h>
#include <impl/linkgraph.h>
#include <util/crudops.h>


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
int linkGraphInit(struct graph_t *g) {
    int retval = GRAPH_ERR;
    if (g != NULL) {
        // Link graphs start with no additional structure
        retval = GRAPH_OK;
    }
    return retval;
}

/**
 * @brief Clear out the underlying data structures for the given LINK graph.
 *
 * @param g Graph to be cleared and memory deallocated
 * @return GRAPH_OK, or GRAPH_ERR when destroyNodes or destroyEdges fails.
 * An empty linked graph has a NULL node list, and destroyNodes returns
 * GRAPH_ERR for that NULL pointer, so freeing an empty linked graph fails.
 */
int linkGraphFree(struct graph_t *g) {
    int retval = GRAPH_OK;
    if (g != NULL) {
        if ((g->gtype & LINKED) == LINKED) {
            struct node_t *currnode = (struct node_t *)g->nodeImpl;
            while (currnode != NULL) {
                if (currnode->edges != NULL &&
                    destroyEdges((void **)&currnode->edges) != GRAPH_OK) {
                    retval = GRAPH_ERR;
                }
                currnode = currnode->next;
            }
            if (destroyNodes(&(g->nodeImpl)) != GRAPH_OK) {
                retval = GRAPH_ERR;
            }
        }
    }
    return retval;
}