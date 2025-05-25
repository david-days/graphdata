//
// Created by david on 10/21/23.
//
#include <impl/sharedmemops.h>
#include <arrayops.h>

/**
 * Shared memory graphs are basically array graphs with the memory segments created in a different way.
 *
 * Therefore, most of the method implementations will simply perform any readonly checks (if necessary) and
 * then call the matching array method directly.
 */

//Read functions to extract data
size_t shmemNodeCount(struct graph_t *g) {
    return arrayNodeCount(g);
}

size_t shmemEdgeCount(struct graph_t *g) {
    return arrayEdgeCount(g);
}

struct node_t * shmemGetNode(const size_t *nodeid, const struct graph_t *g) {
    return arrayGetNode(nodeid, g);
}

int shmemRemoveNode(const size_t *nodeid, struct graph_t *g) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayRemoveNode(nodeid, g);
}

struct edge_t * shmemGetEdge(const size_t *u, const size_t *v, const struct graph_t *g) {
    return arrayGetEdge(u, v, g);
}

struct node_t * shmemGetNeighbors(const size_t *nodeid, const struct graph_t *g) {
    return arrayGetNeighbors(nodeid, g);
}

struct edge_t * shmemGetEdges(const size_t *nodeid, const struct graph_t *g) {
    return arrayGetEdges(nodeid, g);
}

int shmemGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {
    return arrayGetCapacity(uid, vid, cap, g);
}

int shmemGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {
    return arrayGetFlow(uid, vid, flow, g);
}

//Write functions to modify graph
int shmemAddNode(const size_t *nodeid, struct graph_t *g) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayAddNode(nodeid, g);
}

int shmemAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayAddEdge(uid, vid, cap, g);
}

int shmemRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayRemoveEdge(uid, vid, g);
}

int shmemSetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arraySetCapacity(uid, vid, cap, g);
}

int shmemAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayAddCapacity(uid, vid, cap, g);
}

int shmemSetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arraySetFlow(uid, vid, flow, g);
}

int shmemAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayAddFlow(uid, vid, flow, g);
}

int shmemResetGraph(struct graph_t *g, void *args, void (*callback)(void)) {
    //check for readonly
    if ((g->gtype & RD_ONLY) == RD_ONLY) {
        return EXIT_FAILURE;
    }
    return arrayResetGraph(g, args, callback);
}
