/**
 * @brief Operations for a linked-list graph structure.
 *
 * This implementation is for a linked-list graph structure that uses the minimum necessary memory to store
 * vertex and edge data for a graph.  The intent is to support the most general graph usage, wherein the user of the
 * graph structure may not be able to determine exact relationship before graph creation.
 *
 * In this first version, the operations are implemented around a double-linked list structure, with node and edge walking
 * as necessary to find the appropriate relationships.  Future versions will attempt to add more nuanced and intelligent
 * layouts in order to speed up operations.
 */

#include "impl/link/linkops.h"
#include "util/crudops.h"
#include "util/graphcomp.h"



//Read functions to extract data
/**
 * @brief Function pointer definition for getting the node count
 *
 * Walks the node structure to find the total number of nodes in the graph
 * @param g Graph structure in question
 * @return Count of nodes, if graph is not null; otherwise, return 0
 */
size_t linkNodeCount(struct graph_t *g) {
    size_t ncount = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *curr = (struct node_t *)g->nodeImpl;
        while (curr != NULL) {
            ncount++;
            curr = curr->next;
        }
    }
    return ncount;
}

/**
 * @brief Function pointer to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
size_t linkEdgeCount(struct graph_t *g) {
    size_t ecount = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *currnode = (struct node_t *)g->nodeImpl;
        struct edge_t *curredge = NULL;
        while (currnode != NULL) {
            curredge= currnode->edges;
            while (curredge != NULL) {
                ecount++;
                curredge = curredge->next;
            }
            currnode = currnode->next;
        }
    }
    return ecount;
}

/**
 * @brief Function pointer to retrieve a node structure reference.
 *
 * Implementation-specific on whether this
 * structure is part of the original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, pointer to NULL
 */
struct node_t * linkGetNode(const size_t *nodeid, const struct graph_t *g) {
    struct node_t *n = NULL;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *curr = (struct node_t *)g->nodeImpl;
        while (curr != NULL) {
            if (curr->nodeid == *nodeid) {
                n = curr;
                break;
            }
            curr = curr->next;
        }
    }
    return n;
}

/**
 * @brief Function pointer to retrieve a edge structure reference.
 *
 * Walks the structure to find the given edge
 *
 * @param u nodeid of the starting edge.
 * @param v nodeid of the ending edge.
 * @param g Graph structure in question
 * @return pointer to the edge structure, if found; otherwise, pointer to NULL.
 */
struct edge_t * linkGetEdge(const size_t *u, const size_t *v, const struct graph_t *g) {
    struct edge_t *found = NULL;
    if ((g->gtype & LINKED) == LINKED) {
        size_t eu = *u;
        size_t ev = *v;
        if ((g->gtype & UNDIRECTED) == UNDIRECTED) {
            eu = *(minNode((size_t *)u,(size_t *)v));
            ev = *(maxNode((size_t *)u,(size_t *)v));
        }
        struct node_t *node = linkGetNode(&eu,g);
        if (node != NULL) {
            struct edge_t *curr = node->edges;
            while (curr != NULL) {
                if (curr->v == ev) {
                    found = curr;
                    break;
                }
                curr = curr->next;
            }
        }
    }
    return found;
}

/**
 * @brief Function pointer to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.  The edges returned
 * are outgoing neighbors (in the case of a DIRECTED graph).
 *
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of node references, starting with the given node, if found; otherwise, pointer to NULL.
 */
struct node_t * linkGetNeighbors(const size_t *nodeid, const struct graph_t *g) {
    struct node_t *neighbors = NULL;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *n = linkGetNode(nodeid, g);
        if (n != NULL) {
            struct edge_t *curredge = n->edges;
            struct node_t *currnode = NULL;
            while (curredge != NULL) {
                struct node_t *p = initNode();
                p->nodeid = curredge->v;
                if (neighbors == NULL) {
                    neighbors = p;
                } else {
                    p->prev = currnode;
                    currnode->next = p;
                }
                currnode = p;
                curredge = curredge->next;
            }
        }
    }
    return neighbors;
}

/**
 * @brief Function pointer to retrieve linked-list of edges from a given node.
 *
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 *
 * The edges returned are outgoing edges in the case of a DIRECTED graph.
 *
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of edges starting from the given node, if found; otherwise, pointer to NULL.
 */
struct edge_t * linkGetEdges(const size_t *nodeid, const struct graph_t *g) {
    struct edge_t *elist = NULL;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *n = linkGetNode(nodeid, g);
        if (n != NULL) {
            struct edge_t *curr = n->edges;
            struct edge_t *currclone = NULL;
            while (curr != NULL) {
                struct edge_t *eseg = cloneEdge(curr);
                if (elist == NULL) {
                    elist = eseg;
                 } else {
                    eseg->prev = currclone;
                    currclone->next = eseg;
                }
                currclone = eseg;
                curr = curr->next;
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
int linkGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {
    int retval = 0;
    struct edge_t *e = linkGetEdge(uid, vid, g);
    if (e != NULL) {
        *cap = e->cap;
        retval = 1;
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
int linkGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {
    int retval = 0;
    struct edge_t *e = linkGetEdge(uid, vid, g);
    if (e != NULL) {
        *flow = e->flow;
        retval = 1;
    }
    return retval;
}

//Write functions to modify graph
/**
 * @brief Function pointer to add a node to a given graph.
 *
 * Add a new node to the end of the node list
 *
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return 0 if there was an error, 1 if the node was successfully added
 */
int linkAddNode(const size_t *nodeid, struct graph_t *g) {
    int retval = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *exists = linkGetNode(nodeid, g);
        if (exists == NULL) {
            struct node_t *nnode = initNode();
            nnode->nodeid = *nodeid;
            //start walking through to find the correct spot
            struct node_t *curr = (struct node_t *)g->nodeImpl;
            if (curr == NULL) {
                //starting list
                g->nodeImpl = nnode;
            } else {
                while (curr->next != NULL) curr = curr->next;
                curr->next = nnode;
                nnode->prev = curr;
            }
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief Remove a node from the graph.
 *
 * Remove the given node from the list, and repair the linkages appropriately.
 *
 * @param nodeid Node id to be added.
 * @param g Graph structure in question
 * @return 0 if there was an error (node already exists or outside the bounds of the implementation); otherwise, 1 if successful.
 */
int linkRemoveNode(const size_t *nodeid, struct graph_t *g) {
    int retval = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *rnode = linkGetNode(nodeid, g);
        if (rnode != NULL) {
            struct node_t *prev = rnode->prev;
            struct node_t *next = rnode->next;
            //clear out incoming edges
            struct edge_t *curredge = rnode->edges;
            struct edge_t *nextedge = NULL;
            while (curredge != NULL) {
                nextedge = curredge->next;
                linkRemoveEdge(&(curredge->v),nodeid, g);
                curredge = nextedge;
            }
            //Clear outgoing edges
            destroyEdges((void **)&(rnode->edges));
            destroyFeatures((void **)&(rnode->attrs));
            //Cut out node and free memory
            rnode->prev = NULL;
            rnode->next = NULL;
            free(rnode);
            if (prev != NULL) prev->next = next;
            if (next != NULL) next->prev = prev;
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief Function pointer to add an edge to a given graph.
 *
 * Remove the node and repair the linked list, as necessary
 *
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return 0 if there was an error; 1 if the edge was successfully added.
 */
int linkAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g) {
    int retval = 0;
    if ((g->gtype & LINKED) == LINKED) {
        //Is there a node?
        size_t u = *uid;
        size_t v = *vid;
        if ((g->gtype & UNDIRECTED) == UNDIRECTED) {
            u = *(minNode((size_t *)uid, (size_t *)vid));
            v = *(maxNode((size_t *)uid, (size_t *)vid));
        }

        struct node_t *n = linkGetNode(&u, g);
        if (n != NULL) {
            struct edge_t *nedge = initEdge();
            nedge->u = u;
            nedge->v = v;
            nedge->cap = *cap;
            struct edge_t *curr = n->edges;
            //TODO:  Detect if edge already exists
            if (curr == NULL) {
                //starting new edge list
                n->edges = nedge;
            } else {
                //Append to edge list
                while (curr->next != NULL) curr = curr->next;
                curr->next = nedge;
                nedge->prev = curr;
            }
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief Function pointer to remove an edge from the given graph.
 *
 * Allows implementations to remove a given edge from the graph, if necessary.
 *
 * @param uid Identifier for the edge start
 * @param vid Identifier for the edge end.
 * @param g Graph structure in question
 * @return 0 if there was an error (e.g. the edge was not found); otherwise, 1 if the edge was removed.
 */
int linkRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g) {
    int retval = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct edge_t *redge = linkGetEdge(uid, vid, g);
        if (redge != NULL) {
            struct edge_t *prev = redge->prev;
            struct edge_t *next = redge->next;
            destroyFeatures((void **)&(redge->attrs));
            redge->prev = NULL;
            redge->next = NULL;
            redge->cap = 0.0;
            free(redge);
            if (prev != NULL) prev->next = next;
            if (next != NULL) next->prev = prev;
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief Function pointer to set the capacity (cost, weight, etc.) of an edge in the given graph.
 * @param uid identifier of the edge start
 * @param vid identifier of the edge ending.
 * @param cap capacity value to be set
 * @param g Graph structure in question
 * @return 0 if there was an error; 1 if the capacity was successfully set
 */
int linkSetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    int retval = 0;
    struct edge_t *e = linkGetEdge(uid, vid, g);
    if (e != NULL) {
        e->cap = *cap;
        retval = 1;
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
int linkAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    int retval = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct edge_t *e = linkGetEdge(uid, vid, g);
        if (e != NULL) {
            e->cap += *cap;
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief Function to set the flow value for an edge (amount of capacity currently "used")
 *
 * For implementations that require it, this allows tracking of flow values that are used within the graph.
 *
 * @param uid Identifier of the edge start.
 * @param vid Identifier of the edge end.
 * @param flow Value to be set for the flow.
 * @param g Graph structure in question
 * @return 0 of there was an error (edge not found, for example); otherwise, 1 if the flow value as successfully set.
 */
int linkSetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    int retval = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct edge_t *e = linkGetEdge(uid, vid, g);
        if (e != NULL) {
            e->flow = *flow;
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief Function to adjust the flow value of a given edge.
 *
 * For implementations that require it, this allows ajustment of the amount of capacity that is being "used".
 *
 * @param uid Identifier of the edge start
 * @param vid Identifier of the edge end.
 * @param flow The value to be added to adjust the flow value.
 * @param g The graph structure in question
 * @return 0 if there was an error (such as the edge not found); otherwise, 1 if the flow value was successfully adjusted.
 */
int linkAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    int retval = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct edge_t *e = linkGetEdge(uid, vid, g);
        if (e != NULL) {
            e->flow += *flow;
            retval = 1;
        }
    }
    return retval;
}

/**
 * @brief Function pointer to "reset" the graph according to the given argument pointer.
 *
 * Walk the edges and set capacities and flows to zero.
 *
 * @param g Graph structure to be zeroed or modified according to reset logic
 * @param args Arguments to be used in the reset process, if necessary
 * @param callback Callback to be executed when graph has been reset.
 * @return 0 if there was an error during the reset; 1 if the reset completed;
 */
int linkResetGraph(struct graph_t *g, void *args, void (*callback)(void)) {
    int retval = 0;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *currnode = (struct node_t *)g->nodeImpl;
        struct edge_t *curredge = NULL;
        while (currnode != NULL) {
            curredge = currnode->edges;
            while (curredge != NULL) {
                curredge->flow = 0.0;
                curredge->cap = 0.0;
                curredge = curredge->next;
            }
            currnode = currnode->next;
        }
        retval = 1;
    }
    if (callback != NULL) callback();
    return retval;
}

