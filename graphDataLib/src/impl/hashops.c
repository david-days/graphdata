//
// Created by david on 11/18/17.
//

/**
 * @brief Operations for HASHED (chained node hashtable) graphs.
 *
 * Nodes live in prime-sized buckets; edges hang off node_t.edges (LINKED-style).
 * Table starts small and rehashes when nodecount would exceed grow_at.
 */

#include <impl/hashops.h>
#include <impl/hashgraph.h>
#include <util/hashes.h>
#include <util/crudops.h>
#include <stdlib.h>

/**
 * @brief Map a node id into a bucket index.
 */
static size_t hashBucketIndex(size_t nodeid, size_t bucketcount) {
    size_t h = SuperFastHash((const char *)&nodeid, (int)sizeof(size_t));
    return h % bucketcount;
}

/**
 * @brief Rehash into a larger prime-sized bucket array when load requires it.
 *
 * @return GRAPH_OK if no grow needed or grow succeeded; GRAPH_ERR on failure.
 */
static int hashGrowIfNeeded(struct graph_t *g) {
    struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;
    if (meta == NULL || g->nodeImpl == NULL) {
        return GRAPH_ERR;
    }
    if (meta->nodecount + 1u <= meta->grow_at) {
        return GRAPH_OK;
    }

    size_t bound = meta->bucketcount * 2u;
    if (bound <= meta->bucketcount) {
        /* overflow / already huge — refuse rather than corrupt */
        return GRAPH_ERR;
    }
    size_t newcount = maxEratosthenesPrime(&bound);
    if (newcount <= meta->bucketcount) {
        return GRAPH_ERR;
    }

    struct node_t **oldbuckets = (struct node_t **)g->nodeImpl;
    struct node_t **newbuckets = (struct node_t **)calloc(newcount, sizeof(struct node_t *));
    if (newbuckets == NULL) {
        return GRAPH_ERR;
    }

    for (size_t i = 0; i < meta->bucketcount; i++) {
        struct node_t *curr = oldbuckets[i];
        while (curr != NULL) {
            struct node_t *next = curr->next;
            size_t idx = hashBucketIndex(curr->nodeid, newcount);
            curr->prev = NULL;
            curr->next = newbuckets[idx];
            if (newbuckets[idx] != NULL) {
                newbuckets[idx]->prev = curr;
            }
            newbuckets[idx] = curr;
            curr = next;
        }
    }

    free(oldbuckets);
    g->nodeImpl = newbuckets;
    meta->bucketcount = newcount;
    meta->grow_at = (newcount * 3u) / 4u;
    return GRAPH_OK;
}

size_t hashNodeCount(struct graph_t *g) {
    if (g == NULL || (g->gtype & HASHED) != HASHED || g->metaImpl == NULL) {
        return 0;
    }
    return ((struct hashdata_t *)g->metaImpl)->nodecount;
}

size_t hashEdgeCount(struct graph_t *g) {
    if (g == NULL || (g->gtype & HASHED) != HASHED || g->metaImpl == NULL) {
        return 0;
    }
    return ((struct hashdata_t *)g->metaImpl)->edgecount;
}

struct node_t * hashGetNode(const size_t *nodeid, const struct graph_t *g) {
    if (nodeid == NULL || g == NULL || (g->gtype & HASHED) != HASHED) {
        return NULL;
    }
    struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;
    struct node_t **buckets = (struct node_t **)g->nodeImpl;
    if (meta == NULL || buckets == NULL || meta->bucketcount == 0) {
        return NULL;
    }

    size_t idx = hashBucketIndex(*nodeid, meta->bucketcount);
    struct node_t *curr = buckets[idx];
    while (curr != NULL) {
        if (curr->nodeid == *nodeid) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/**
 * @brief Find outgoing edge to v on a node's adjacency list.
 */
static struct edge_t *findEdgeOnNode(const struct node_t *node, size_t v) {
    if (node == NULL) {
        return NULL;
    }
    struct edge_t *curr = node->edges;
    while (curr != NULL) {
        if (curr->v == v) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/**
 * @brief Append a new directed edge record to a node's list.
 *
 * @return GRAPH_OK on success; GRAPH_ERR on allocation failure.
 */
static int appendEdgeOnNode(struct node_t *node, size_t u, size_t v, double cap) {
    struct edge_t *nedge = initEdge();
    if (nedge == NULL) {
        return GRAPH_ERR;
    }
    nedge->u = u;
    nedge->v = v;
    nedge->cap = cap;

    if (node->edges == NULL) {
        node->edges = nedge;
    } else {
        struct edge_t *curr = node->edges;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = nedge;
        nedge->prev = curr;
    }
    return GRAPH_OK;
}

/**
 * @brief Unlink and free one edge from a node's list; does not touch meta edgecount.
 */
static void unlinkEdgeOnNode(struct node_t *node, struct edge_t *redge) {
    if (node == NULL || redge == NULL) {
        return;
    }
    struct edge_t *prev = redge->prev;
    struct edge_t *next = redge->next;
    destroyFeatures((void **)&(redge->attrs));
    redge->prev = NULL;
    redge->next = NULL;
    free(redge);
    if (prev != NULL) {
        prev->next = next;
    } else {
        node->edges = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
}

/**
 * @brief Remove one directed record (u→v) and decrement edgecount if found.
 */
static int removeOneDirected(struct graph_t *g, size_t u, size_t v) {
    struct node_t *node = hashGetNode(&u, g);
    struct edge_t *redge = findEdgeOnNode(node, v);
    if (node == NULL || redge == NULL) {
        return GRAPH_ERR;
    }
    unlinkEdgeOnNode(node, redge);
    struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;
    if (meta != NULL && meta->edgecount > 0) {
        meta->edgecount--;
    }
    return GRAPH_OK;
}

struct edge_t * hashGetEdge(const size_t *u, const size_t *v, const struct graph_t *g) {
    if (u == NULL || v == NULL || g == NULL || (g->gtype & HASHED) != HASHED) {
        return NULL;
    }
    struct node_t *node = hashGetNode(u, g);
    return findEdgeOnNode(node, *v);
}

struct node_t * hashGetNeighbors(const size_t *nodeid, const struct graph_t *g) {
    if (nodeid == NULL || g == NULL || (g->gtype & HASHED) != HASHED) {
        return NULL;
    }
    struct node_t *n = hashGetNode(nodeid, g);
    if (n == NULL) {
        return NULL;
    }

    struct node_t *neighbors = NULL;
    struct node_t *currnode = NULL;
    struct edge_t *curredge = n->edges;
    while (curredge != NULL) {
        struct node_t *p = initNode();
        if (p == NULL) {
            destroyNodes((void **)&neighbors);
            return NULL;
        }
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
    return neighbors;
}

struct edge_t * hashGetEdges(const size_t *nodeid, const struct graph_t *g) {
    if (nodeid == NULL || g == NULL || (g->gtype & HASHED) != HASHED) {
        return NULL;
    }
    struct node_t *n = hashGetNode(nodeid, g);
    if (n == NULL) {
        return NULL;
    }

    struct edge_t *elist = NULL;
    struct edge_t *currclone = NULL;
    struct edge_t *curr = n->edges;
    while (curr != NULL) {
        struct edge_t *eseg = cloneEdge(curr);
        if (eseg == NULL) {
            destroyEdges((void **)&elist);
            return NULL;
        }
        if (elist == NULL) {
            elist = eseg;
        } else {
            eseg->prev = currclone;
            currclone->next = eseg;
        }
        currclone = eseg;
        curr = curr->next;
    }
    return elist;
}

int hashGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {
    if (cap == NULL) {
        return GRAPH_ERR;
    }
    struct edge_t *e = hashGetEdge(uid, vid, g);
    if (e == NULL) {
        return GRAPH_ERR;
    }
    *cap = e->cap;
    return GRAPH_OK;
}

int hashGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {
    if (flow == NULL) {
        return GRAPH_ERR;
    }
    struct edge_t *e = hashGetEdge(uid, vid, g);
    if (e == NULL) {
        return GRAPH_ERR;
    }
    *flow = e->flow;
    return GRAPH_OK;
}

int hashAddNode(const size_t *nodeid, struct graph_t *g) {
    if (nodeid == NULL || g == NULL || (g->gtype & HASHED) != HASHED) {
        return GRAPH_ERR;
    }
    if (hashGetNode(nodeid, g) != NULL) {
        return GRAPH_ERR;
    }
    if (hashGrowIfNeeded(g) != GRAPH_OK) {
        return GRAPH_ERR;
    }

    struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;
    struct node_t **buckets = (struct node_t **)g->nodeImpl;
    if (meta == NULL || buckets == NULL) {
        return GRAPH_ERR;
    }

    struct node_t *nnode = initNode();
    if (nnode == NULL) {
        return GRAPH_ERR;
    }
    nnode->nodeid = *nodeid;

    size_t idx = hashBucketIndex(*nodeid, meta->bucketcount);
    nnode->next = buckets[idx];
    if (buckets[idx] != NULL) {
        buckets[idx]->prev = nnode;
    }
    buckets[idx] = nnode;
    meta->nodecount++;
    return GRAPH_OK;
}

int hashRemoveNode(const size_t *nodeid, struct graph_t *g) {
    if (nodeid == NULL || g == NULL || (g->gtype & HASHED) != HASHED
        || g->metaImpl == NULL || g->nodeImpl == NULL) {
        return GRAPH_ERR;
    }

    struct node_t *rnode = hashGetNode(nodeid, g);
    if (rnode == NULL) {
        return GRAPH_ERR;
    }

    /* Drop adjacency involving this node (outgoing first; undirected remove clears reverse). */
    while (rnode->edges != NULL) {
        size_t v = rnode->edges->v;
        if (hashRemoveEdge(nodeid, &v, g) != GRAPH_OK) {
            /* Fall back: unlink one directed record if removeEdge failed */
            if (removeOneDirected(g, *nodeid, v) != GRAPH_OK) {
                return GRAPH_ERR;
            }
        }
    }

    /* Directed: also strip incoming edges from other nodes. */
    if (GRAPH_IS_DIRECTED(g->gtype)) {
        struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;
        struct node_t **buckets = (struct node_t **)g->nodeImpl;
        for (size_t i = 0; i < meta->bucketcount; i++) {
            struct node_t *curr = buckets[i];
            while (curr != NULL) {
                struct node_t *nextnode = curr->next;
                if (curr->nodeid != *nodeid) {
                    while (findEdgeOnNode(curr, *nodeid) != NULL) {
                        if (removeOneDirected(g, curr->nodeid, *nodeid) != GRAPH_OK) {
                            return GRAPH_ERR;
                        }
                    }
                }
                curr = nextnode;
            }
        }
    }

    struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;
    struct node_t **buckets = (struct node_t **)g->nodeImpl;
    size_t idx = hashBucketIndex(*nodeid, meta->bucketcount);
    struct node_t *prev = rnode->prev;
    struct node_t *next = rnode->next;
    if (prev != NULL) {
        prev->next = next;
    } else {
        buckets[idx] = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }

    destroyFeatures((void **)&(rnode->attrs));
    rnode->prev = NULL;
    rnode->next = NULL;
    rnode->edges = NULL;
    free(rnode);

    if (meta->nodecount > 0) {
        meta->nodecount--;
    }
    return GRAPH_OK;
}

int hashAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g) {
    if (uid == NULL || vid == NULL || cap == NULL || g == NULL
        || (g->gtype & HASHED) != HASHED || g->metaImpl == NULL) {
        return GRAPH_ERR;
    }

    size_t u = *uid;
    size_t v = *vid;
    struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;

    if (GRAPH_IS_UNDIRECTED(g->gtype)) {
        struct node_t *unode = hashGetNode(&u, g);
        struct node_t *vnode = hashGetNode(&v, g);
        if (unode == NULL || vnode == NULL) {
            return GRAPH_ERR;
        }
        if (findEdgeOnNode(unode, v) != NULL || findEdgeOnNode(vnode, u) != NULL) {
            return GRAPH_ERR;
        }

        if (appendEdgeOnNode(unode, u, v, *cap) != GRAPH_OK) {
            return GRAPH_ERR;
        }
        meta->edgecount++;

        /* Self-loop: one record only */
        if (u != v) {
            if (appendEdgeOnNode(vnode, v, u, *cap) != GRAPH_OK) {
                removeOneDirected(g, u, v);
                return GRAPH_ERR;
            }
            meta->edgecount++;
        }
        return GRAPH_OK;
    }

    /* Directed: require start node; reject duplicates */
    struct node_t *unode = hashGetNode(&u, g);
    if (unode == NULL) {
        return GRAPH_ERR;
    }
    if (findEdgeOnNode(unode, v) != NULL) {
        return GRAPH_ERR;
    }
    if (appendEdgeOnNode(unode, u, v, *cap) != GRAPH_OK) {
        return GRAPH_ERR;
    }
    meta->edgecount++;
    return GRAPH_OK;
}

int hashRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g) {
    if (uid == NULL || vid == NULL || g == NULL || (g->gtype & HASHED) != HASHED) {
        return GRAPH_ERR;
    }

    size_t u = *uid;
    size_t v = *vid;

    if (GRAPH_IS_UNDIRECTED(g->gtype)) {
        int a = removeOneDirected(g, u, v);
        if (u != v) {
            int b = removeOneDirected(g, v, u);
            /* OK if at least one side existed (tolerates partial state) */
            return (a == GRAPH_OK || b == GRAPH_OK) ? GRAPH_OK : GRAPH_ERR;
        }
        return a;
    }

    return removeOneDirected(g, u, v);
}

int hashSetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    if (cap == NULL) {
        return GRAPH_ERR;
    }
    struct edge_t *e = hashGetEdge(uid, vid, g);
    if (e == NULL) {
        return GRAPH_ERR;
    }
    e->cap = *cap;
    if (g != NULL && GRAPH_IS_UNDIRECTED(g->gtype) && uid != NULL && vid != NULL && *uid != *vid) {
        struct edge_t *rev = hashGetEdge(vid, uid, g);
        if (rev != NULL) {
            rev->cap = *cap;
        }
    }
    return GRAPH_OK;
}

int hashAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    if (cap == NULL) {
        return GRAPH_ERR;
    }
    struct edge_t *e = hashGetEdge(uid, vid, g);
    if (e == NULL) {
        return GRAPH_ERR;
    }
    e->cap += *cap;
    if (g != NULL && GRAPH_IS_UNDIRECTED(g->gtype) && uid != NULL && vid != NULL && *uid != *vid) {
        struct edge_t *rev = hashGetEdge(vid, uid, g);
        if (rev != NULL) {
            rev->cap += *cap;
        }
    }
    return GRAPH_OK;
}

int hashSetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    if (flow == NULL) {
        return GRAPH_ERR;
    }
    struct edge_t *e = hashGetEdge(uid, vid, g);
    if (e == NULL) {
        return GRAPH_ERR;
    }
    e->flow = *flow;
    if (g != NULL && GRAPH_IS_UNDIRECTED(g->gtype) && uid != NULL && vid != NULL && *uid != *vid) {
        struct edge_t *rev = hashGetEdge(vid, uid, g);
        if (rev != NULL) {
            rev->flow = *flow;
        }
    }
    return GRAPH_OK;
}

int hashAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    if (flow == NULL) {
        return GRAPH_ERR;
    }
    struct edge_t *e = hashGetEdge(uid, vid, g);
    if (e == NULL) {
        return GRAPH_ERR;
    }
    e->flow += *flow;
    if (g != NULL && GRAPH_IS_UNDIRECTED(g->gtype) && uid != NULL && vid != NULL && *uid != *vid) {
        struct edge_t *rev = hashGetEdge(vid, uid, g);
        if (rev != NULL) {
            rev->flow += *flow;
        }
    }
    return GRAPH_OK;
}

int hashResetGraph(struct graph_t *g, void *args, void (*callback)(void)) {
    (void)args;
    if (g == NULL || (g->gtype & HASHED) != HASHED
        || g->metaImpl == NULL || g->nodeImpl == NULL) {
        if (callback != NULL) {
            callback();
        }
        return GRAPH_ERR;
    }

    struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;
    struct node_t **buckets = (struct node_t **)g->nodeImpl;
    for (size_t i = 0; i < meta->bucketcount; i++) {
        struct node_t *currnode = buckets[i];
        while (currnode != NULL) {
            struct edge_t *curredge = currnode->edges;
            while (curredge != NULL) {
                curredge->flow = 0.0;
                curredge->cap = 0.0;
                curredge = curredge->next;
            }
            currnode = currnode->next;
        }
    }

    if (callback != NULL) {
        callback();
    }
    return GRAPH_OK;
}
