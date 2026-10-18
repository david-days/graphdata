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

#include <impl/linkops.h>
#include <util/crudops.h>
#include <util/graphcomp.h>



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
 * @brief Live linked node, or NULL if it is absent.
 *
 * The pointer is inside the graph. Callers must not free it.
 * linkGetNode clones this result.
 */
static struct node_t * linkFindNode(const size_t *nodeid, const struct graph_t *g) {
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
 * @brief Live linked edge, or NULL if it is absent.
 *
 * The pointer is inside the graph. Callers must not free it.
 * linkGetEdge clones this result.
 */
static struct edge_t * linkFindEdge(const size_t *u, const size_t *v, const struct graph_t *g) {
    struct edge_t *found = NULL;
    if ((g->gtype & LINKED) == LINKED) {
        size_t eu = *u;
        size_t ev = *v;
        if (GRAPH_IS_UNDIRECTED(g->gtype)) {
            eu = *(minNode((size_t *)u,(size_t *)v));
            ev = *(maxNode((size_t *)u,(size_t *)v));
        }
        struct node_t *node = linkFindNode(&eu,g);
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

struct node_t * linkGetNode(const size_t *nodeid, const struct graph_t *g) {
    return cloneNode(linkFindNode(nodeid, g));
}

struct edge_t * linkGetEdge(const size_t *u, const size_t *v, const struct graph_t *g) {
    return cloneEdge(linkFindEdge(u, v, g));
}

/**
 * @brief Function pointer to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Caller-owned neighbor ids taken from this node's edges (each edge's v).
 * On a directed graph those are the outgoing neighbors. The queried node is
 * not included. Release the chain with destroyNodes. NULL if the node is
 * absent or has no edges.
 */
struct node_t * linkGetNeighbors(const size_t *nodeid, const struct graph_t *g) {
    struct node_t *neighbors = NULL;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *n = linkFindNode(nodeid, g);
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
 * Caller-owned copies of this node's edges. On a directed graph those are
 * the outgoing edges. Release the chain with destroyEdges. NULL if the node
 * is absent or has no edges.
 */
struct edge_t * linkGetEdges(const size_t *nodeid, const struct graph_t *g) {
    struct edge_t *elist = NULL;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *n = linkFindNode(nodeid, g);
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {
    int retval = GRAPH_ERR;
    struct edge_t *e = linkFindEdge(uid, vid, g);
    if (e != NULL) {
        *cap = e->cap;
        retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {
    int retval = GRAPH_ERR;
    struct edge_t *e = linkFindEdge(uid, vid, g);
    if (e != NULL) {
        *flow = e->flow;
        retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkAddNode(const size_t *nodeid, struct graph_t *g) {
    int retval = GRAPH_ERR;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *exists = linkFindNode(nodeid, g);
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
            retval = GRAPH_OK;
        }
    }
    return retval;
}

/**
 * @brief Remove a node from the graph.
 *
 * Remove the given node from the list, and repair the linkages appropriately.
 *
 * @param nodeid Node id to be removed.
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkRemoveNode(const size_t *nodeid, struct graph_t *g) {
    int retval = GRAPH_ERR;
    if ((g->gtype & LINKED) == LINKED) {
        struct node_t *rnode = linkFindNode(nodeid, g);
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
            if (prev != NULL) {
                prev->next = next;
            } else {
                // Removed the list head
                g->nodeImpl = next;
            }
            if (next != NULL) {
                next->prev = prev;
            }
            retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g) {
    int retval = GRAPH_ERR;
    if ((g->gtype & LINKED) == LINKED) {
        //Is there a node?
        size_t u = *uid;
        size_t v = *vid;
        if (GRAPH_IS_UNDIRECTED(g->gtype)) {
            u = *(minNode((size_t *)uid, (size_t *)vid));
            v = *(maxNode((size_t *)uid, (size_t *)vid));
        }

        struct node_t *n = linkFindNode(&u, g);
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
            retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g) {
    int retval = GRAPH_ERR;
    if ((g->gtype & LINKED) == LINKED) {
        size_t eu = *uid;
        if (GRAPH_IS_UNDIRECTED(g->gtype)) {
            eu = *(minNode((size_t *)uid, (size_t *)vid));
        }
        struct node_t *node = linkFindNode(&eu, g);
        struct edge_t *redge = linkFindEdge(uid, vid, g);
        if (node != NULL && redge != NULL) {
            struct edge_t *prev = redge->prev;
            struct edge_t *next = redge->next;
            destroyFeatures((void **)&(redge->attrs));
            redge->prev = NULL;
            redge->next = NULL;
            redge->cap = 0.0;
            free(redge);
            if (prev != NULL) {
                prev->next = next;
            } else {
                // Removed the head of this node's edge list
                node->edges = next;
            }
            if (next != NULL) {
                next->prev = prev;
            }
            retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkSetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    int retval = GRAPH_ERR;
    struct edge_t *e = linkFindEdge(uid, vid, g);
    if (e != NULL) {
        e->cap = *cap;
        retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    int retval = GRAPH_ERR;
    if ((g->gtype & LINKED) == LINKED) {
        struct edge_t *e = linkFindEdge(uid, vid, g);
        if (e != NULL) {
            e->cap += *cap;
            retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkSetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    int retval = GRAPH_ERR;
    if ((g->gtype & LINKED) == LINKED) {
        struct edge_t *e = linkFindEdge(uid, vid, g);
        if (e != NULL) {
            e->flow = *flow;
            retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    int retval = GRAPH_ERR;
    if ((g->gtype & LINKED) == LINKED) {
        struct edge_t *e = linkFindEdge(uid, vid, g);
        if (e != NULL) {
            e->flow += *flow;
            retval = GRAPH_OK;
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
int linkResetGraph(struct graph_t *g, void *args, void (*callback)(void)) {
    int retval = GRAPH_ERR;
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
        retval = GRAPH_OK;
    }
    if (callback != NULL) callback();
    return retval;
}

int linkForeachNode(const struct graph_t *g, int (*visit)(const size_t *nodeid, void *ctx), void *ctx) {
    struct node_t *curr = NULL;
    if (g == NULL || visit == NULL || (g->gtype & LINKED) != LINKED) {
        return GRAPH_ERR;
    }
    curr = (struct node_t *)g->nodeImpl;
    while (curr != NULL) {
        struct node_t *next = curr->next;
        size_t nodeid = curr->nodeid;
        if (visit(&nodeid, ctx) != GRAPH_OK) {
            return GRAPH_ERR;
        }
        curr = next;
    }
    return GRAPH_OK;
}

struct node_t * linkListNodes(const struct graph_t *g) {
    return listNodesByWalk(g, linkForeachNode);
}

