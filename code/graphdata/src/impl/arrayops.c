/**
 * @brief These are the implementations for the basic graph operations that are part of
 * the graphops_t structure.
 *
 *
 */
#include <impl/arraygraph.h>
#include <impl/arrayops.h>
#include <util/graphcomp.h>
#include <stdlib.h>

/**
 * @brief Search the array for a given edge.
 *
 * If the edge is found, set the value in the index pointer and return 1.  Otherwise,
 * return 0.
 * @param u Pointer to start node value of the edge
 * @param v Pointer to the end node value of the edge
 * @param idx Index value to be set, if found
 * @param g Graph structure data
 * @return 1 if the edge was found and the idx value set; otherwise, 0.
 */
static int findEdgeOffset(const size_t *u, const size_t *v, size_t *offset, const struct graph_t *g) {
    int found = 0;
    struct arraydata_t *gmeta = (struct arraydata_t *)g->metaImpl;
    size_t *nodearr = (size_t *)g->nodeImpl;
    size_t conn = gmeta->degree;
    for (size_t i = 0; i < conn;i++) {
        if (*(nodearr+*u+i) == *v) {
            *offset = i;
            found = 1;
            break;
        }
    }
    return found;
}

static int zeroDoubleArray(size_t ecount, size_t conncount, double *darr) {
    int retval = 0;
    if (darr != NULL) {
        //TODO:  Prevent overwriting outside array?
        for (size_t i = 0;i<ecount*conncount;i++) {
            *(darr + i) = 0;
        }
        retval = 1;
    }
    return retval;
}


//Read functions to extract data
/**
 * @brief Implementation for getting the node count;
 * @param g Graph structure in question
 * @return Count of nodes, if graph is not null; otherwise, return 0
 */
size_t arrayNodeCount(struct graph_t *g) {
    size_t ncount = 0;
    struct arraydata_t *mdata = (struct arraydata_t *)g->metaImpl;
    ncount = mdata->nodelen;
    return ncount;
}

/**
 * @brief Implementation to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
size_t arrayEdgeCount(struct graph_t *g) {
    size_t ecount = 0;
    struct arraydata_t *mdata = (struct arraydata_t *)g->metaImpl;
    ecount = mdata->edgelen;
    return ecount;
}

/**
 * @brief Implementation to retrieve a node structure reference.
 *
 * If the struct returned is not NULL, the consumer is responsible for calling free() on the memory.
 *
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, pointer to NULL
 */
struct node_t * arrayGetNode(const size_t *nodeid, const struct graph_t *g){
    struct node_t *node = NULL;
    if (g->metaImpl != NULL) {
        struct arraydata_t *meta = (struct arraydata_t *)g->metaImpl;
        if (*nodeid < meta->nodelen) {
            node = (struct node_t *)malloc(sizeof(struct node_t));
            if (node != NULL) {
                node->nodeid = *nodeid;
                node->attrs = NULL;
                node->edges = NULL;
                node->next = NULL;
                node->prev = NULL;
            } else {
                free(node);
                //TODO:  Anything else to ensure cleanup?
                node = NULL;
            }
        }
    }
    return node;
}

/**
 * @brief Implementation to retrieve a edge structure reference.
 *
 * Implementation-specific on whether this structure is part of the
 * original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param u nodeid of the starting edge.
 * @param v nodeid of the ending edge.
 * @param g Graph structure in question
 * @return pointer to the edge structure, if found; otherwise, pointer to NULL.
 */
struct edge_t * arrayGetEdge(const size_t *u, const size_t *v, const struct graph_t *g){
    struct edge_t *edge = NULL;
    size_t eOffset = 0;
    if (findEdgeOffset(u,v, &eOffset, g) && g->capImpl != NULL) {
        edge = (struct edge_t *)malloc(sizeof(struct edge_t));
        if (edge != NULL) {
            edge->u = *u;
            edge->v = *v;
            if (g->capImpl != NULL) {
                double *caparr = (double *) g->capImpl;
                edge->cap = *(caparr + *u + eOffset);
            } else {
                edge->cap = 0.0;
            }
            if (g->flowImpl != NULL) {
                double *farr = (double *)g->flowImpl;
                edge->flow = *(farr + *u + eOffset);
            } else {
                edge->flow = 0.0;
            }

            edge->attrs = NULL;
            edge->prev = NULL;
            edge->next = NULL;

        }
    }
    return NULL;
}

/**
 * @brief Implementation to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of node references, starting with the given node, if found; otherwise, pointer to NULL.
 */
struct node_t * arrayGetNeighbors(const size_t *nodeid, const struct graph_t *g) {
    struct node_t *nlist = NULL;
    if (g->metaImpl != NULL && g->nodeImpl != NULL) {
        struct arraydata_t *meta = (struct arraydata_t *)g->metaImpl;
        size_t *nodarr = (size_t *)g->nodeImpl;
        //Make sure we have valid array index
        if (*nodeid < meta->nodelen) {
            struct node_t *curr = NULL;
            size_t nOffset = 0;
            while (nOffset < meta->degree) {
                if (*(nodarr + *nodeid + nOffset) != 0) {
                    struct node_t *neighbor = malloc(sizeof(struct node_t));
                    if (neighbor != NULL) {
                        neighbor->prev = curr;
                        neighbor->next = NULL;
                        neighbor->nodeid = *(nodarr + *nodeid + nOffset);
                        neighbor->attrs = NULL;
                        if (curr != NULL) {
                            curr->next = neighbor;
                        }
                        curr = neighbor;
                        if (nlist == NULL) nlist = curr;
                        nOffset++;
                    }
                }
            }
        }
    }
    return nlist;
}

/**
 * @brief Implementation to retrieve linked-list of edges from a given node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of edges starting from the given node, if found; otherwise, pointer to NULL.
 */
struct edge_t * arrayGetEdges(const size_t *nodeid, const struct graph_t *g) {
    struct edge_t *elist = NULL;
    if (g->metaImpl != NULL && g->nodeImpl != NULL) {
        if (g->metaImpl != NULL && g->nodeImpl != NULL) {
            struct arraydata_t *meta = (struct arraydata_t *)g->metaImpl;
            size_t *nodarr = (size_t *)g->nodeImpl;
            double *caparr = (double *)g->capImpl;
            double *farr = (double *)g->flowImpl;
            //Make sure we have valid array index
            if (*nodeid < meta->nodelen && caparr != NULL && farr != NULL) {
                struct edge_t *curr = NULL;
                size_t nOffset = 0;
                while (nOffset < meta->degree) {
                    if (*(nodarr + *nodeid + nOffset) != 0) {
                        struct edge_t *edge = malloc(sizeof(struct node_t));
                        if (edge != NULL) {
                            edge->prev = curr;
                            edge->next = NULL;
                            edge->u = *nodeid;
                            edge->v = *(nodarr + *nodeid + nOffset);
                            edge->cap = *(caparr + *nodeid + nOffset);
                            edge->flow = *(farr + *nodeid + nOffset);
                            edge->attrs = NULL;
                            if (curr != NULL) {
                                curr->next = edge;
                            }
                            curr = edge;
                            if (elist == NULL) elist = curr;
                            nOffset++;
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

/**
 * @brief Implementation to retrieve linked-list path of nodes from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of nodes, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
struct node_t * arrayGetNodePath(const size_t *uid, const size_t *vid, const struct graph_t *g){
    return NULL;
}

/**
 * @brief Implementation to retrieve linked-list of path edges from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of edges, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
struct edge_t * arrayGetEdgePath(const size_t *uid, const size_t *vid, const struct graph_t *g) {
    return NULL;
}


//Write functions to modify graph
/**
 * @brief Implementation to add a node to a given graph.
 * NOOP implementation for array-based graphs--the node counts are fixed at creation.
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return 0 if there was an error, 1 if the node was successfully added
 */
int arrayAddNode(const size_t *nodeid, struct graph_t *g){
    return 0;
}

/**
 * @brief Implementation to add an edge to a given graph.
 *
 * Adds a connecting-edge referece (if not already in place, and the degree of the graph allows it (there are unfilled spots available
 * for the given uid).
 *
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return 0 if there was an error; 1 if the edge was successfully added.
 */
int arrayAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g) {
    int added = 0;
    if (g != NULL) {
        if (g->metaImpl != NULL) {
            struct arraydata_t *meta = (struct arradata_t *)g->metaImpl;
            const size_t *u = minNode(uid, vid);
            const size_t *v = maxNode(uid, vid);
            size_t *nodarr = (size_t *)g->nodeImpl;
            double *caparr = (double *)g->capImpl;
            double *farr = (double *)g->flowImpl;
            if (*u < meta->nodelen) {
                size_t offset = 0;
                while (!added && offset < meta->degree) {
                    if (*(nodarr + *u + offset) == 0) {
                        *(nodarr + *u + offset) = *v;
                        *(caparr + *u + offset) = *cap;
                        *(farr + *u + offset) = 0.0;
                        added = 1;
                    }
                }

            }


        }
    }
    return added;
}

/**
 * @brief Implementation to set the capacity (cost, weight, etc.) of an edge in the given graph.
 * @param uid identifier of the edge start
 * @param vid identifier of the edge ending.
 * @param cap capacity value to be set
 * @param g Graph structure in question
 * @return 0 if there was an error; 1 if the capacity was successfully set
 */
int arraySetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g){
    int retval = 0;
    size_t eOffset = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (g->capImpl != NULL) {
        if (findEdgeOffset(u, v, &eOffset, g)) {
            double *caparr = (double *)g->capImpl;
            *(caparr + *u + eOffset) = *cap;
            retval = 1;
        }
    }
    return retval;

}


/**
 * @brief Implementation to "reset" the graph according to the given argument pointer.
 *
 * For this implementation, the args value is ignored and the edge values are set to 0.0 again.
 *
 * @param g Graph structure to be zeroed or modified according to reset logic
 * @param args Arguments to be((arraydata_t *)g->metaImpl used in the reset process, if necessary
 * @param callback Callback to be executed when graph has been reset.
 * @return 0 if there was an error during the reset; 1 if the reset completed;
 */
int arrayResetGraph(struct graph_t *g, void *args, void (*callback)(void)) {
    int retval = 0;
    struct arraydata_t *gmeta = (struct arraydata_t *)g->metaImpl;
    if (gmeta != NULL) {
        retval = zeroDoubleArray(gmeta->edgelen, gmeta->degree, (double *)g->capImpl);
        retval = retval & zeroDoubleArray(gmeta->edgelen, gmeta->degree, (double *)g->flowImpl);
    }
    return retval;
}

/**
 * @brief Utility method to free up an array
 *
 * Deallocates all unerlying arrays and the containing array.
 * @param arraylen Number of elements in the array
 * @param arr 2D array to be cleared
 * @return 1 if successful; 0 if error.
 */
int freeGraphArray(size_t arraylen, void** arrptr) {
    int retval = 0;
    if (*arrptr != NULL) {
        free(*arrptr);
        *arrptr = NULL;
        retval = 1;
    }
    return retval;
}