//
// Created by david on 10/21/23.
//

#include <graphData.h>
#include <impl/memmap/mmapgraph.h>
#include <impl/memmap/mmapops.h>
#include <util/graphcomp.h>
#include <sys/mman.h>

/**
 * @brief Search the mmap graph for a given edge.
 *
 * If the edge is found, set the value in the index pointer and return 1.  Otherwise,
 * return 0.
 * @param u Pointer to start node value of the edge
 * @param v Pointer to the end node value of the edge
 * @param idx Index value to be set, if found
 * @param g Graph structure data
 * @return 1 if the edge was found and the idx value set; otherwise, 0.
 */
static short findMMapEdgeOffset(const size_t *u, const size_t *v, size_t *index, size_t *offset, const struct graph_t *g) {
    short found = OP_FAIL;
    struct mmapdata_t *gmeta = (struct mmapdata_t *)g->metaImpl;
    size_t *nodearr = (size_t *)g->nodeImpl;
    size_t conn = gmeta->degree;
    size_t idx = *u * conn;
    for (size_t i = 0; i < conn;i++) {
        if (*(nodearr+idx+i) == *v) {
            *index = idx;
            *offset = i;
            found = OP_SUCCESS;
            break;
        }
    }
    return found;
}


static short zeroMMapDouble(size_t ecount, size_t conncount, double *darr) {
    short retval = OP_FAIL;
    if (darr != NULL) {
        //TODO:  Prevent overwriting outside array?
        for (size_t i = 0;i<ecount*conncount;i++) {
            *(darr + i) = 0;
        }
        retval = OP_SUCCESS;
    }
    return retval;
}
//Read functions to extract data
/**
 * @brief Implementation for getting the node count;
 * @param g Graph structure in question
 * @return Count of nodes, if graph is not null; otherwise, return 0
 */
size_t mmapNodeCount(struct graph_t *g) {
    size_t ncount = 0;
    struct mmapdata_t *mdata = (struct mmapdata_t *)g->metaImpl;
    ncount = mdata->nodelen;
    return ncount;
}
/**
 * @brief Implementation to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
size_t mmapEdgeCount(struct graph_t *g) {
    size_t ecount = 0;
    struct mmapdata_t *mdata = (struct mmapdata_t *)g->metaImpl;
    ecount = mdata->edgelen;
    return ecount;
}
/**
 * @brief Implementation to retrieve a node structure reference.
 *
 * Implementation-specific on whether this
 * structure is part of the original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, pointer to NULL
 */
struct node_t * mmapGetNode(const size_t *nodeid, const struct graph_t *g) {
    struct node_t *node = NULL;
    if (g->metaImpl != NULL) {
        struct mmapdata_t *meta = (struct mmapdata_t *)g->metaImpl;
        size_t idx = *nodeid * meta->degree;
        if (idx < meta->nodelen) {
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
 * @brief Remove a node from the graph.
 *
 * NOOP implementation.  ARRAY-based graphs are defined at initialization and do not allow removing nodes.
 *
 * @param nodeid Node id to be added.
 * @param g Graph structure in question
 * @return 0 if there was an error (node already exists or outside the bounds of the implementation); otherwise, 1 if successful.
 */
short mmapRemoveNode(const size_t *nodeid, struct graph_t *g) {
    return OP_FAIL;
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
struct edge_t * mmapGetEdge(const size_t *u, const size_t *v, const struct graph_t *g) {
    struct edge_t *edge = NULL;
    size_t eOffset = 0;
    size_t eIdx = 0;
    if (findMMapEdgeOffset(u,v, &eIdx, &eOffset, g) == OP_SUCCESS && g->capImpl != NULL && g->metaImpl != NULL) {
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
struct node_t * mmapGetNeighbors(const size_t *nodeid, const struct graph_t *g) {
    struct node_t *nlist = NULL;
    if (g->metaImpl != NULL && g->nodeImpl != NULL) {
        struct mmapdata_t *meta = (struct mmapdata_t *)g->metaImpl;
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
struct edge_t * mmapGetEdges(const size_t *nodeid, const struct graph_t *g) {
    struct edge_t *elist = NULL;
    if (g->metaImpl != NULL && g->nodeImpl != NULL) {
        if (g->metaImpl != NULL && g->nodeImpl != NULL) {
            struct mmapdata_t *meta = (struct mmapdata_t *)g->metaImpl;
            size_t nidx = *nodeid * meta->degree;
            size_t *nodarr = (size_t *)g->nodeImpl;
            double *caparr = (double *)g->capImpl;
            double *farr = (double *)g->flowImpl;
            //Make sure we have valid array index
            if (nidx < meta->nodelen && caparr != NULL && farr != NULL) {
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

    return elist;

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
short mmapGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {
    short retval = OP_FAIL;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (g->capImpl != NULL && g->metaImpl != NULL) {
        if (findMMapEdgeOffset(u, v, &eIdx, &eOffset, g) == OP_SUCCESS) {
            double *caparr = (double *)g->capImpl;
            *cap = *(caparr + eIdx + eOffset);
            retval = OP_SUCCESS;
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
short mmapGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {
    short retval = OP_FAIL;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (g->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (g->flowImpl != NULL && g->metaImpl != NULL) {
        if (findMMapEdgeOffset(u, v, &eIdx, &eOffset, g) == OP_SUCCESS) {
            double *farr = (double *)g->flowImpl;
            *flow = *(farr + eIdx + eOffset) ;
            retval = OP_SUCCESS;
        }
    }
    return retval;

}


//Write functions to modify graph
/**
 * @brief Implementation to add a node to a given graph.
 * Not all implementations may use this (for example, fixed-size graphs such as spatial ARRAY implementations).
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return 0 if there was an error, 1 if the node was successfully added
 */
short mmapAddNode(const size_t *nodeid, struct graph_t *g){
    return OP_FAIL;
}
/**
 * @brief Implementation to add an edge to a given graph.
 * Not all implementations may use this (for example, fixed-size ARRAY implementations representing a set domain of nodes and relationships).
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return 0 if there was an error; 1 if the edge was successfully added.
 */
short mmapAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *graph) {
    short added = OP_FAIL;
    if (graph != NULL) {
        if (graph->metaImpl != NULL) {
            struct mmapdata_t *meta = (struct mmapdata_t *)graph->metaImpl;
            const size_t *u = minNode((size_t *)uid, (size_t *)vid);
            const size_t *v = maxNode((size_t *)uid, (size_t *)vid);
            size_t nidx = *u * meta->degree;
            size_t *nodarr = (size_t *)graph->nodeImpl;
            double *caparr = (double *)graph->capImpl;
            double *farr = (double *)graph->flowImpl;
            if (*u < meta->nodelen) {
                size_t offset = 0;
                while (!added && offset < meta->degree) {
                    if (*(nodarr + nidx + offset) == 0) {
                        *(nodarr + nidx + offset) = *v;
                        *(caparr + nidx + offset) = *cap;
                        *(farr + nidx + offset) = 0.0;
                        added = OP_SUCCESS;
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
short mmapRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *graph) {
    short removed = OP_FAIL;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (graph->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (graph->metaImpl != NULL) {
        if (findMMapEdgeOffset(u, v, &eIdx, &eOffset, graph) == OP_SUCCESS) {
            size_t *narr = (size_t *)graph->nodeImpl;
            *(narr + eIdx + eOffset) = 0;
            if (graph->capImpl != NULL) {
                double *caparr = (double *)graph->capImpl;
                *(caparr + eIdx + eOffset) = 0.0;
            }
            if (graph->flowImpl != NULL) {
                double *farr = (double *)graph->flowImpl;
                *(farr + eIdx + eOffset) = 0.0;
            }
            removed = OP_SUCCESS;
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
short mmapSetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *graph) {
    short retval = OP_FAIL;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (graph->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (graph->capImpl != NULL && graph->metaImpl != NULL) {
        if (findMMapEdgeOffset(u, v, &eIdx, &eOffset, graph) == OP_SUCCESS) {
            double *caparr = (double *)graph->capImpl;
            *(caparr + eIdx + eOffset) = *cap;
            retval = OP_SUCCESS;
        }
    }
    return retval;

}

/**
 * @brief Function to add (adjust) the capacity for an edge by a given amount.
 *
 * Add the capacity value to the current capacity for the given edge.
 *
 * @param uid Identifier of the edge start
 * @param vid Identifier of the edge end.
 * @param cap Value to adjust the capacity
 * @param g Graph structure in question
 * @return 0 if there was an error (edge not found, for example); 1 of capacity was successfully adjusted
 */
short mmapAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *graph) {
    short retval = OP_FAIL;
    size_t eIdx = 0;
    size_t eOffset = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (graph->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *) uid, (size_t *)vid);
    }
    if (graph->capImpl != NULL && graph->metaImpl != NULL) {
        if (findMMapEdgeOffset(u, v, &eIdx, &eOffset, graph) == OP_SUCCESS) {
            double *caparr = (double *)graph->capImpl;
            *(caparr + eIdx + eOffset) += *cap;
            retval = OP_SUCCESS;
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
short mmapSetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *graph) {
    short retval = OP_FAIL;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (graph->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *)uid, (size_t *)vid);
    }

    if (graph->flowImpl != NULL && graph->metaImpl != NULL) {
        if (findMMapEdgeOffset(u, v, &eIdx, &eOffset, graph) == OP_SUCCESS) {
            double *farr = (double *)graph->flowImpl;
            *(farr + eIdx + eOffset) = *flow;
            retval = OP_SUCCESS;
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
short mmapAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *graph) {
    short retval = OP_FAIL;
    size_t eIdx = 0;
    size_t eOffset = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    if (graph->gtype == UNDIRECTED) {
        u = minNode((size_t *)uid, (size_t *)vid);
        v = maxNode((size_t *) uid, (size_t *)vid);
    }
    if (graph->flowImpl != NULL && graph->metaImpl != NULL) {
        if (findMMapEdgeOffset(u, v, &eIdx, &eOffset, graph) == OP_SUCCESS) {
            double *farr = (double *)graph->flowImpl;
            *(farr + eIdx + eOffset) += *flow;
            retval = OP_SUCCESS;
        }
    }
    return retval;
}


/**
 * @brief Implementation to "reset" the graph according to the given argument pointer.
 *
 * For some implementations, it is more efficient to reuse the existing graph structure and perform a "zero-out"
 * of the data, rather than rebuilding from scratch.  This function pointer provides that option.
 * @param g Graph structure to be zeroed or modified according to reset logic
 * @param args Arguments to be used in the reset process, if necessary
 * @param callback Callback to be executed when graph has been reset.
 * @return 0 if there was an error during the reset; 1 if the reset completed;
 */
short mmapResetGraph(struct graph_t *graph, void *args, void (*callback)(void)) {
    short retval = OP_SUCCESS;
    struct mmapdata_t *gmeta = (struct mmapdata_t *)graph->metaImpl;
    if (gmeta != NULL) {
        retval = zeroMMapDouble(gmeta->edgelen, gmeta->degree, (double *)graph->capImpl);
        retval = retval | zeroMMapDouble(gmeta->edgelen, gmeta->degree, (double *)graph->flowImpl);
    }
    return retval;
}

/**
 * @brief Utility method to free up mmap
 *
 * Deallocates all underlying memory-map structure.
 * @param arraylen Number of elements in the array
 * @param arr 2D array to be cleared
 * @return 1 if successful; 0 if error.
 */
int freeGraphMMap(size_t memlen, void** mmapptr) {
    short retval = OP_FAIL;
    if (*mmapptr != NULL) {
        munmap(mmapptr, memlen);
        *mmapptr = NULL;
        retval = OP_SUCCESS;
    }
    return retval;
}

