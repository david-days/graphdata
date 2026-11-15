/**
 * Shortest path on a six-node directed graph.
 *
 *   10 -> 20 -> 50 -> 70 -> 90
 *   10 -> 30 -> 70
 *   30 -> 50
 *
 * The shortest path from 10 to 90 is 10 -> 30 -> 70 -> 90, distance 3.
 * LINKED and HASHED store those six ids. ARRAY, SHARED_MEM, and SHARED_MMAP
 * are a fixed 10 by 10 index (degree 2) that merely contains them.
 *
 * Against an install:
 *   gcc bfs_demo.c bfs_shortest_path.c $(pkg-config --cflags --libs graphdata)
 */

#include "bfs_shortest_path.h"

#include <stdio.h>
#include <stdlib.h>

static const size_t kNodes[] = {10, 20, 30, 50, 70, 90};
static const size_t kEdges[][2] = {
    {10, 20}, {10, 30}, {20, 50}, {30, 50}, {30, 70}, {50, 70}, {70, 90}
};
static const size_t kExpect[] = {10, 30, 70, 90};

static int loadGraph(struct graphops_t *ops, struct graph_t *g) {
    double cap = 1.0;
    for (size_t i = 0; i < sizeof(kNodes) / sizeof(kNodes[0]); i++) {
        size_t id = kNodes[i];
        if (ops->addNode(&id, g) != GRAPH_OK) {
            return GRAPH_ERR;
        }
    }
    for (size_t i = 0; i < sizeof(kEdges) / sizeof(kEdges[0]); i++) {
        size_t u = kEdges[i][0];
        size_t v = kEdges[i][1];
        if (ops->addEdge(&u, &v, &cap, g) != GRAPH_OK) {
            return GRAPH_ERR;
        }
    }
    return GRAPH_OK;
}

static int pathMatches(const size_t *path, size_t length) {
    if (length != sizeof(kExpect) / sizeof(kExpect[0])) {
        return 0;
    }
    for (size_t i = 0; i < length; i++) {
        if (path[i] != kExpect[i]) {
            return 0;
        }
    }
    return 1;
}

static int runOne(const char *name, enum GRAPHDOMAIN impl, struct dimensions_t *dims) {
    struct graph_t *g = initGraph(impl | DIRECTED | GENERIC, 0, dims);
    struct graphops_t *ops = NULL;
    size_t path[8];
    size_t length = 0;
    int ok = 0;

    if (g == NULL) {
        fprintf(stderr, "%s: initGraph failed\n", name);
        return 0;
    }
    ops = getOperations(g);
    if (ops == NULL || loadGraph(ops, g) != GRAPH_OK
        || bfsShortestPath(ops, g, 10, 90, path, 8, &length) != GRAPH_OK
        || !pathMatches(path, length)) {
        fprintf(stderr, "%s: expected 10 -> 30 -> 70 -> 90\n", name);
    } else {
        printf("%s: 10 -> 30 -> 70 -> 90 (distance %zu)\n", name, length - 1);
        ok = 1;
    }

    destroyGraphops((void **)&ops);
    clearGraph(g);
    destroyGraph((void **)&g);
    return ok;
}

int main(void) {
    struct dimensions_t *dims = createDimensions(2, (size_t)10, (size_t)10);
    int ok = 1;

    if (dims == NULL) {
        return EXIT_FAILURE;
    }
    ok = runOne("LINKED", LINKED, NULL);
    ok = runOne("HASHED", HASHED, NULL) && ok;
    ok = runOne("ARRAY", ARRAY, dims) && ok;
    ok = runOne("SHARED_MEM", SHARED_MEM, dims) && ok;
    ok = runOne("SHARED_MMAP", SHARED_MMAP, dims) && ok;
    destroyDimensions((void **)&dims);
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
