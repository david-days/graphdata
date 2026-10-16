/**
 * @brief These are the implementations for the basic graph operations that are part of
 * the graphops_t structure.
 *
 *
 */
#include <impl/arraygraph.h>
#include <impl/arrayops.h>
#include <util/graphcomp.h>
#include <util/crudops.h>

/**
 * @brief Search the array for a given edge.
 *
 * If the edge is found, set the index/offset pointers and return GRAPH_TRUE.
 * Otherwise return GRAPH_FALSE.
 */
static bool findEdgeOffset(const size_t *u, const size_t *v, size_t *index, size_t *offset, const struct graph_t *g) {
    struct arraydata_t *gmeta = (struct arraydata_t *)g->metaImpl;
    size_t *nodearr = (size_t *)g->nodeImpl;
    size_t conn = gmeta->degree;
    size_t idx = *u * conn;
    for (size_t i = 0; i < conn; i++) {
        if (*(nodearr + idx + i) == *v) {
            *index = idx;
            *offset = i;
            return GRAPH_TRUE;
        }
    }
    return GRAPH_FALSE;
}

/**
 * Canonicalize undirected endpoints to (min, max); leave directed as-is.
 */
static void orientEndpoints(const size_t *uid, const size_t *vid, const struct graph_t *g,
                            const size_t **u_out, const size_t **v_out) {
    if (GRAPH_IS_UNDIRECTED(g->gtype)) {
        *u_out = minNode((size_t *)uid, (size_t *)vid);
        *v_out = maxNode((size_t *)uid, (size_t *)vid);
    } else {
        *u_out = uid;
        *v_out = vid;
    }
}

static int zeroDoubleArray(size_t ecount, size_t conncount, double *darr) {
    if (darr == NULL) {
        return GRAPH_ERR;
    }
    for (size_t i = 0; i < ecount * conncount; i++) {
        *(darr + i) = 0.0;
    }
    return GRAPH_OK;
}

//Read functions to extract data
size_t arrayNodeCount(struct graph_t *g) {
    size_t ncount = 0;
    struct arraydata_t *mdata = (struct arraydata_t *)g->metaImpl;
    ncount = mdata->nodelen;
    return ncount;
}

size_t arrayEdgeCount(struct graph_t *g) {
    size_t ecount = 0;
    struct arraydata_t *mdata = (struct arraydata_t *)g->metaImpl;
    ecount = mdata->edgelen;
    return ecount;
}

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
            }
        }
    }
    return node;
}

struct edge_t * arrayGetEdge(const size_t *u, const size_t *v, const struct graph_t *g){
    struct edge_t *edge = NULL;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *ou = u;
    const size_t *ov = v;
    orientEndpoints(u, v, g, &ou, &ov);

    if (findEdgeOffset(ou, ov, &eIdx, &eOffset, g) && g->capImpl != NULL && g->metaImpl != NULL) {
        edge = (struct edge_t *)malloc(sizeof(struct edge_t));
        if (edge != NULL) {
            edge->u = *ou;
            edge->v = *ov;
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
    return edge;
}

struct node_t * arrayGetNeighbors(const size_t *nodeid, const struct graph_t *g) {
    struct node_t *nlist = NULL;
    if (g->metaImpl != NULL && g->nodeImpl != NULL) {
        struct arraydata_t *meta = (struct arraydata_t *)g->metaImpl;
        size_t *nodarr = (size_t *)g->nodeImpl;
        size_t nidx = *nodeid * meta->degree;
        if (*nodeid < meta->nodelen) {
            struct node_t *curr = NULL;
            for (size_t nOffset = 0; nOffset < meta->degree; nOffset++) {
                size_t neighborId = *(nodarr + nidx + nOffset);
                if (neighborId == GRAPH_SLOT_EMPTY) {
                    continue;
                }
                struct node_t *neighbor = malloc(sizeof(struct node_t));
                if (neighbor == NULL) {
                    destroyNodes((void **)&nlist);
                    return NULL;
                }
                neighbor->prev = curr;
                neighbor->next = NULL;
                neighbor->nodeid = neighborId;
                neighbor->attrs = NULL;
                neighbor->edges = NULL;
                if (curr != NULL) {
                    curr->next = neighbor;
                }
                curr = neighbor;
                if (nlist == NULL) {
                    nlist = curr;
                }
            }
        }
    }
    return nlist;
}

struct edge_t * arrayGetEdges(const size_t *nodeid, const struct graph_t *g) {
    struct edge_t *elist = NULL;
    if (g->metaImpl != NULL && g->nodeImpl != NULL) {
        struct arraydata_t *meta = (struct arraydata_t *)g->metaImpl;
        size_t nidx = *nodeid * meta->degree;
        size_t *nodarr = (size_t *)g->nodeImpl;
        double *caparr = (double *)g->capImpl;
        double *farr = (double *)g->flowImpl;
        if (*nodeid < meta->nodelen && caparr != NULL && farr != NULL) {
            struct edge_t *curr = NULL;
            for (size_t nOffset = 0; nOffset < meta->degree; nOffset++) {
                size_t neighborId = *(nodarr + nidx + nOffset);
                if (neighborId == GRAPH_SLOT_EMPTY) {
                    continue;
                }
                struct edge_t *edge = malloc(sizeof(struct edge_t));
                if (edge == NULL) {
                    destroyEdges((void **)&elist);
                    return NULL;
                }
                edge->prev = curr;
                edge->next = NULL;
                edge->u = *nodeid;
                edge->v = neighborId;
                edge->cap = *(caparr + nidx + nOffset);
                edge->flow = *(farr + nidx + nOffset);
                edge->attrs = NULL;
                if (curr != NULL) {
                    curr->next = edge;
                }
                curr = edge;
                if (elist == NULL) {
                    elist = curr;
                }
            }
        }
    }

    return elist;
}

int arrayGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {
    int retval = GRAPH_ERR;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    orientEndpoints(uid, vid, g, &u, &v);

    if (g->capImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *caparr = (double *)g->capImpl;
            *cap = *(caparr + eIdx + eOffset);
            retval = GRAPH_OK;
        }
    }
    return retval;
}

int arrayGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {
    int retval = GRAPH_ERR;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    orientEndpoints(uid, vid, g, &u, &v);

    if (g->flowImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *farr = (double *)g->flowImpl;
            *flow = *(farr + eIdx + eOffset);
            retval = GRAPH_OK;
        }
    }
    return retval;
}


//Write functions to modify graph
int arrayAddNode(const size_t *nodeid, struct graph_t *g){
    (void)nodeid;
    (void)g;
    return GRAPH_OK;
}

int arrayRemoveNode(const size_t *nodeid, struct graph_t *g) {
    (void)nodeid;
    (void)g;
    return GRAPH_OK;
}

int arrayAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g) {
    int added = GRAPH_ERR;
    if (g != NULL && g->metaImpl != NULL) {
        struct arraydata_t *meta = (struct arraydata_t *)g->metaImpl;
        const size_t *u = uid;
        const size_t *v = vid;
        orientEndpoints(uid, vid, g, &u, &v);
        size_t nidx = *u * meta->degree;
        size_t *nodarr = (size_t *)g->nodeImpl;
        double *caparr = (double *)g->capImpl;
        double *farr = (double *)g->flowImpl;
        if (*u < meta->nodelen && nodarr != NULL && caparr != NULL && farr != NULL) {
            // Already present?
            size_t eIdx = 0;
            size_t eOffset = 0;
            if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
                *(caparr + eIdx + eOffset) = *cap;
                return GRAPH_OK;
            }
            for (size_t offset = 0; offset < meta->degree; offset++) {
                if (*(nodarr + nidx + offset) == GRAPH_SLOT_EMPTY) {
                    *(nodarr + nidx + offset) = *v;
                    *(caparr + nidx + offset) = *cap;
                    *(farr + nidx + offset) = 0.0;
                    added = GRAPH_OK;
                    break;
                }
            }
        }
    }
    return added;
}

int arrayRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g) {
    int removed = GRAPH_ERR;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    orientEndpoints(uid, vid, g, &u, &v);

    if (g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            size_t *narr = (size_t *)g->nodeImpl;
            *(narr + eIdx + eOffset) = GRAPH_SLOT_EMPTY;
            if (g->capImpl != NULL) {
                double *caparr = (double *)g->capImpl;
                *(caparr + eIdx + eOffset) = 0.0;
            }
            if (g->flowImpl != NULL) {
                double *farr = (double *)g->flowImpl;
                *(farr + eIdx + eOffset) = 0.0;
            }
            removed = GRAPH_OK;
        }
    }
    return removed;
}

int arraySetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g){
    int retval = GRAPH_ERR;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    orientEndpoints(uid, vid, g, &u, &v);

    if (g->capImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *caparr = (double *)g->capImpl;
            *(caparr + eIdx + eOffset) = *cap;
            retval = GRAPH_OK;
        }
    }
    return retval;
}

int arrayAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g){
    int retval = GRAPH_ERR;
    size_t eIdx = 0;
    size_t eOffset = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    orientEndpoints(uid, vid, g, &u, &v);

    if (g->capImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *caparr = (double *)g->capImpl;
            *(caparr + eIdx + eOffset) += *cap;
            retval = GRAPH_OK;
        }
    }
    return retval;
}

int arraySetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g){
    int retval = GRAPH_ERR;
    size_t eOffset = 0;
    size_t eIdx = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    orientEndpoints(uid, vid, g, &u, &v);

    if (g->flowImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *farr = (double *)g->flowImpl;
            *(farr + eIdx + eOffset) = *flow;
            retval = GRAPH_OK;
        }
    }
    return retval;
}

int arrayAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g){
    int retval = GRAPH_ERR;
    size_t eIdx = 0;
    size_t eOffset = 0;
    const size_t *u = uid;
    const size_t *v = vid;
    orientEndpoints(uid, vid, g, &u, &v);

    if (g->flowImpl != NULL && g->metaImpl != NULL) {
        if (findEdgeOffset(u, v, &eIdx, &eOffset, g)) {
            double *farr = (double *)g->flowImpl;
            *(farr + eIdx + eOffset) += *flow;
            retval = GRAPH_OK;
        }
    }
    return retval;
}

int arrayResetGraph(struct graph_t *g, void *args, void (*callback)(void)) {
    (void)args;
    int retval = GRAPH_ERR;
    struct arraydata_t *gmeta = (struct arraydata_t *)g->metaImpl;
    if (gmeta != NULL) {
        int cap_ok = zeroDoubleArray(gmeta->edgelen, gmeta->degree, (double *)g->capImpl);
        int flow_ok = zeroDoubleArray(gmeta->edgelen, gmeta->degree, (double *)g->flowImpl);
        if (cap_ok == GRAPH_OK && flow_ok == GRAPH_OK) {
            retval = GRAPH_OK;
            if (callback != NULL) {
                callback();
            }
        }
    }
    return retval;
}

int freeGraphArray(size_t arraylen, void** arrptr) {
    (void)arraylen;
    int retval = EXIT_SUCCESS;
    if (*arrptr != NULL) {
        free(*arrptr);
        *arrptr = NULL;
        retval = EXIT_SUCCESS;
    }
    return retval;
}
