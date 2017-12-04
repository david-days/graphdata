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
static int findEdgeOffset(const size_t *u, const size_t *v, size_t *index, size_t *offset, const struct graph_t *g) {
    int found = 0;
    struct arraydata_t *gmeta = (struct arraydata_t *)g->metaImpl;
    size_t *nodearr = (size_t *)g->nodeImpl;
    size_t conn = gmeta->degree;
    size_t idx = *u * conn;
    for (size_t i = 0; i < conn;i++) {
        if (*(nodearr+idx+i) == *v) {
            *index = idx;
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
        size_t idx = *nodeid * meta->degree;
        if (idx < meta->arraylen) {
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
    size_t eIdx = 0;
    if (findEdgeOffset(u,v, &eIdx, &eOffset, g) && g->capImpl != NULL && g->metaImpl != NULL) {
        edge = (struct edge_t *)malloc(sizeof(struct edge_t));
        if (edge != NULL) {
            edge->u = *u;
            edge->v = *v;
            if (g->capImpl != NULL) {
                double *caparr = (double *) g->capImpl;
                edge->cap = *(caparr + eIdx + eOffset);
            } else {
                edge->cap = 0.0;
            }
            if (g->flowImpl != NULL) {
                double *farr = (double *)g->flowImpl;
                edge->flow = *(farr + eIdx + eOffset);
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
        size_t nidx = *nodeid * meta->degree;
        //Make sure we have valid array index
        if (*nodeid < meta->nodelen) {
            struct node_t *curr = NULL;
            size_t nOffset = 0;
            while (nOffset < meta->degree) {
                if (*(nodarr + nidx + nOffset) != 0) {
                    struct node_t *neighbor = malloc(sizeof(struct node_t));
                    if (neighbor != NULL) {
                        neighbor->prev = curr;
                        neighbor->next = NULL;
                        neighbor->nodeid = *(nodarr + nidx + nOffset);
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
            size_t nidx = *nodeid * meta->degree;
            size_t *nodarr = (size_t *)g->nodeImpl;
            double *caparr = (double *)g->capImpl;
            double *farr = (double *)g->flowImpl;
            //Make sure we have valid array index
            if (nidx < meta->arraylen && caparr != NULL && farr != NULL) {
                struct edge_t *curr = NULL;
                size_t nOffset = 0;
                while (nOffset < meta->degree) {
                    if (*(nodarr + nidx + nOffset) != 0) {
                        struct edge_t *edge = malloc(sizeof(struct node_t));
                        if (edge != NULL) {
                            edge->prev = curr;
                            edge->next = NULL;
                            edge->u = *nodeid;
                            edge->v = *(nodarr + nidx + nOffset);
                            edge->cap = *(caparr + nidx + nOffset);
                            edge->flow = *(farr + nidx + nOffset);
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
 * @brief Function pointer to retrieve the current capacity value for a given edge.
 *
 * For implementations that support this, the capacity value will be written to the *cap parameter.
 *
 * @param uid Edge start identifier
 * @param vid Edge end identifier
 * @param cap Capacity value pointer to store the value
 * @param g Graph structure in question
 * @return 0 if there was a problem retrieving the value (such as the edge not existing); otherwise, 1 for a successful
 * retrieval
 */
int arrayGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {
    int retval = 0;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (g->capImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *caparr = (double *)g->capImpl;
            *cap = *(caparr + eIdx + eOffset);
            retval = 1;
        }
    }
    return retval;

}

/**
 * @brief Function pointer to retrieve the current flow value for a given edge.
 *
 * For implementations that support this, the flow value will be written to the *flow parameter.
 *
 * @param uid Edge start identifier
 * @param vid Edge end identifier
 * @param flow Flow value pointer to store the result
 * @param g Graph structure in question
 * @return 0 if there was a problem retrieving the value (such as the edge not existing); otherwise, 1 for a successful
 * retrieval
 */
int arrayGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {
    int retval = 0;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (g->flowImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *farr = (double *)g->flowImpl;
            *flow = *(farr + eIdx + eOffset) ;
            retval = 1;
        }
    }
    return retval;

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
 * @brief Remove a node from the graph.
 *
 * NOOP implementation.  ARRAY-based graphs are defined at initialization and do not allow removing nodes.
 *
 * @param nodeid Node id to be added.
 * @param g Graph structure in question
 * @return 0 if there was an error (node already exists or outside the bounds of the implementation); otherwise, 1 if successful.
 */
int arrayRemoveNode(const size_t *nodeid, struct graph_t *g) {
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
            struct arraydata_t *meta = (struct arraydata_t *)g->metaImpl;
            const size_t *u = minNode((size_t *)uid, (size_t *)vid);
            const size_t *v = maxNode((size_t *)uid, (size_t *)vid);
            size_t nidx = *u * meta->degree;
            size_t *nodarr = (size_t *)g->nodeImpl;
            double *caparr = (double *)g->capImpl;
            double *farr = (double *)g->flowImpl;
            if (*u < meta->nodelen) {
                size_t offset = 0;
                while (!added && offset < meta->degree) {
                    if (*(nodarr + nidx + offset) == 0) {
                        *(nodarr + nidx + offset) = *v;
                        *(caparr + nidx + offset) = *cap;
                        *(farr + nidx + offset) = 0.0;
                        added = 1;
                    }
                }

            }


        }
    }
    return added;
}

/**
 * @brief Function pointer to remove an edge from the given graph.
 *
 * Remove references for an edge within the structure.
 *
 * @param uid Identifier for the edge start
 * @param vid Identifier for the edge end.
 * @param g Graph structure in question
 * @return 0 if there was an error (e.g. the edge was not found); otherwise, 1 if the edge was removed.
 */
int arrayRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g) {
    int removed = 0;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            size_t *narr = (size_t *)g->nodeImpl;
            *(narr + eIdx + eOffset) = 0;
            if (g->capImpl != NULL) {
                double *caparr = (double *)g->capImpl;
                *(caparr + eIdx + eOffset) = 0.0;
            }
            if (g->flowImpl != NULL) {
                double *farr = (double *)g->flowImpl;
                *(farr + eIdx + eOffset) = 0.0;
            }
            removed = 1;
        }
    }
    return removed;
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
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (g->capImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *caparr = (double *)g->capImpl;
            *(caparr + eIdx + eOffset) = *cap;
            retval = 1;
        }
    }
    return retval;

}


/**
 * @brief Function to add (adjust) the capacity for an edge by a given amount.
 *
 * For implementations that require it, this function allows the altering of a specified edge capacity by the given amount.
 *
 * @param uid Identifier of the edge start
 * @param vid Identifier of the edge end.
 * @param cap Value to adjust the capacity
 * @param g Graph structure in question
 * @return 0 if there was an error (edge not found, for example); 1 of capacity was successfully adjusted
 */
int arrayAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g){
    int retval = 0;
    size_t eIdx = 0;
    size_t eOffset = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *) uid, (size_t *)vid);
    }
    if (g->capImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u,v,&eIdx, &eOffset, g)) {
            double *caparr = (double *)g->capImpl;
            *(caparr + eIdx + eOffset) += *cap;
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief Function to set the flow value for an edge (amount of capacity currently "used")
 *
 * Set the flow value for the given edge.
 *
 * @param uid Identifier of the edge start.
 * @param vid Identifier of the edge end.
 * @param flow Value to be set for the flow.
 * @param g Graph structure in question
 * @return 0 of there was an error (edge not found, for example); otherwise, 1 if the flow value as successfully set.
 */
int arraySetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g){
    int retval = 0;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (g->flowImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *farr = (double *)g->flowImpl;
            *(farr + eIdx + eOffset) = *flow;
            retval = 1;
        }
    }
    return retval;

}

/**
 * @brief Function to adjust the flow value of a given edge.
 *
 * Add the flow value to the flow tracking of the given graph.
 *
 * @param uid Identifier of the edge start
 * @param vid Identifier of the edge end.
 * @param flow The value to be added to adjust the flow value.
 * @param g The graph structure in question
 * @return 0 if there was an error (such as the edge not found); otherwise, 1 if the flow value was successfully adjusted.
 */
int arrayAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g){
    int retval = 0;
    size_t eIdx = 0;
    size_t eOffset = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *) uid, (size_t *)vid);
    }
    if (g->flowImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u,v,&eIdx, &eOffset, g)) {
            double *farr = (double *)g->flowImpl;
            *(farr + eIdx + eOffset) += *flow;
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