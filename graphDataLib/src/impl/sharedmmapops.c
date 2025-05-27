//
// Created by david on 10/21/23.
//

#include <impl/sharedmmapops.h>
#include <arrayops.h>

#include <sharedmmapgraph.h>

/**
 * Shared memory graphs are basically array graphs with the memory segments created in a different way.
 *
 * Therefore, most of the method implementations will simply perform any readonly checks (if necessary) and
 * then call the matching array method directly.
 */

//Read functions to extract data
size_t shmapNodeCount(struct graph_t *g) {
    return arrayNodeCount(g);
}

size_t shmapEdgeCount(struct graph_t *g) {
    return arrayEdgeCount(g);
}

struct node_t * shmapGetNode(const size_t *nodeid, const struct graph_t *g) {
    return arrayGetNode(nodeid, g);
}

int shmapRemoveNode(const size_t *nodeid, struct graph_t *g) {
    //check for readonly
    struct shmmapdata_t * meta = g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayRemoveNode(nodeid, g);
}

struct edge_t * shmapGetEdge(const size_t *u, const size_t *v, const struct graph_t *g) {
    return arrayGetEdge(u, v, g);
}

struct node_t * shmapGetNeighbors(const size_t *nodeid, const struct graph_t *g) {
    return arrayGetNeighbors(nodeid, g);
}

struct edge_t * shmapGetEdges(const size_t *nodeid, const struct graph_t *g) {
    return arrayGetEdges(nodeid, g);
}

int shmapGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {
    return arrayGetCapacity(uid, vid, cap, g);
}

int shmapGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {
    return arrayGetFlow(uid, vid, flow, g);
}

//Write functions to modify graph
int shmapAddNode(const size_t *nodeid, struct graph_t *g) {
    //check for readonly
    struct shmmapdata_t * meta = (struct shmapdata_t *)g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayAddNode(nodeid, g);
}

int shmapAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g) {
    //check for readonly
    struct shmmapdata_t * meta = (struct shmapdata_t *)g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayAddEdge(uid, vid, cap, g);
}

int shmapRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g) {
    //check for readonly
    struct shmmapdata_t * meta = (struct shmapdata_t *)g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayRemoveEdge(uid, vid, g);
}

int shmapSetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    //check for readonly
    struct shmmapdata_t * meta = (struct shmapdata_t *)g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arraySetCapacity(uid, vid, cap, g);
}

int shmapAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    //check for readonly
    struct shmmapdata_t * meta = (struct shmapdata_t *)g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayAddCapacity(uid, vid, cap, g);
}

int shmapSetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    //check for readonly
    struct shmmapdata_t * meta = (struct shmapdata_t *)g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arraySetFlow(uid, vid, flow, g);
}

int shmapAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    //check for readonly
    struct shmmapdata_t * meta = (struct shmapdata_t *)g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayAddFlow(uid, vid, flow, g);
}

int shmapResetGraph(struct graph_t *g, void *args, void (*callback)(void)) {
    //check for readonly
    struct shmmapdata_t * meta = (struct shmapdata_t *)g->metaImpl;
    if (meta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayResetGraph(g, args, callback);
}
