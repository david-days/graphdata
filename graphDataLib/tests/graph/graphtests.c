//
// Created by david on 12/3/17.
//


#include <check.h>
#include <graphInit.h>
#include <graphOps.h>
#include <util/crudops.h>
#include <stdlib.h>
#include <util/cartesian.h>


#define ARRAY_DIM_CUBE 10
#define ARRAY_DIM_SMALL 3
#define ARRAY_CAP_VAL 55.0
#define ARRAY_FLOW_VAL 12.5
#define LINK_CAP_VAL 37.0
#define LINK_FLOW_VAL 4.0
#define LINK_NODE_COUNT 6

/**
 * Fill linked graph according to LINK_NODE_COUNT
 */
void fillLinkTestGraph(struct graph_t *g, struct graphops_t *gops) {

    for (size_t i = 0; i < LINK_NODE_COUNT; i++) {
        gops->addNode(&i, g);
    }
    double cap = LINK_CAP_VAL;
    for (size_t u = 0; u < LINK_NODE_COUNT; u++) {
        for (size_t v = 0; v < LINK_NODE_COUNT; v++) {
            if (u != v) {
                gops->addEdge(&u, &v, &cap, g);
            }
        }
    }
}

/**
 * Count nodes in a returned neighbor list and free it.
 */
static size_t countAndFreeNodes(struct node_t *list) {
    size_t count = 0;
    struct node_t *curr = list;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    destroyNodes((void **)&list);
    return count;
}

/**
 * Count edges in a returned edge list and free it.
 */
static size_t countAndFreeEdges(struct edge_t *list) {
    size_t count = 0;
    struct edge_t *curr = list;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    destroyEdges((void **)&list);
    return count;
}


/**
 * @brief Test basic operations for the array graph structure
 */
START_TEST(arrayGraphTest) {

        struct dimensions_t *dims = createDimensions(3, ARRAY_DIM_CUBE, ARRAY_DIM_CUBE, ARRAY_DIM_CUBE);
        ck_assert(dims != NULL);
        struct graph_t *g = initGraph(ARRAY | UNDIRECTED | GENERIC, 0, dims);
        ck_assert(g != NULL);
        ck_assert(g->dims == dims);
        struct graphops_t *gops = getOperations(g);
        ck_assert(gops != NULL);
        size_t ncount = gops->nodeCount(g);
        size_t nexpected = cartesianIndexLength(dims);
        ck_assert(ncount == nexpected);
        int opsclear = destroyGraphops((void **)&gops);
        ck_assert(opsclear == GRAPH_OK);
        ck_assert(gops == NULL);
        int graphclear = clearGraph(g);
        ck_assert(graphclear == GRAPH_OK);
        int deallocsuccess = destroyGraph((void **)&g);
        ck_assert(deallocsuccess == GRAPH_OK);
        ck_assert(g == NULL);

}
END_TEST

/**
 * Capacity and flow round-trips on an array graph with explicit edges.
 */
START_TEST(arrayCapFlowTest) {
    struct dimensions_t *dims = createDimensions(3, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL);
    ck_assert(dims != NULL);
    struct graph_t *g = initGraph(ARRAY | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t u = 0;
    size_t v = 1;
    size_t w = 2;
    double cap = ARRAY_CAP_VAL;
    double flow = ARRAY_FLOW_VAL;
    double got = 0.0;

    ck_assert(gops->addEdge(&u, &v, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&u, &w, &cap, g) == GRAPH_OK);

    // Undirected: either endpoint order should resolve
    ck_assert(gops->getCapacity(&v, &u, &got, g) == GRAPH_OK);
    ck_assert(got == ARRAY_CAP_VAL);

    double newcap = 99.0;
    ck_assert(gops->setCapacity(&u, &v, &newcap, g) == GRAPH_OK);
    ck_assert(gops->getCapacity(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == 99.0);

    double delta = 1.0;
    ck_assert(gops->addCapacity(&u, &v, &delta, g) == GRAPH_OK);
    ck_assert(gops->getCapacity(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == 100.0);

    ck_assert(gops->setFlow(&u, &v, &flow, g) == GRAPH_OK);
    ck_assert(gops->getFlow(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == ARRAY_FLOW_VAL);

    ck_assert(gops->addFlow(&u, &v, &delta, g) == GRAPH_OK);
    ck_assert(gops->getFlow(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == ARRAY_FLOW_VAL + 1.0);

    struct edge_t *e = gops->getEdge(&u, &v, g);
    ck_assert(e != NULL);
    ck_assert(e->cap == 100.0);
    ck_assert(e->flow == ARRAY_FLOW_VAL + 1.0);
    free(e);

    // Missing edge fails cleanly
    size_t missing = 5;
    ck_assert(gops->getCapacity(&u, &missing, &got, g) == GRAPH_ERR);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * Neighbor / edge-list retrieval for array graphs.
 */
START_TEST(arrayNeighborTest) {
    struct dimensions_t *dims = createDimensions(3, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL);
    ck_assert(dims != NULL);
    struct graph_t *g = initGraph(ARRAY | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t u = 0;
    size_t v = 1;
    size_t w = 2;
    double cap = ARRAY_CAP_VAL;
    ck_assert(gops->addEdge(&u, &v, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&u, &w, &cap, g) == GRAPH_OK);

    struct node_t *neighbors = gops->getNeighbors(&u, g);
    ck_assert(neighbors != NULL);
    ck_assert(countAndFreeNodes(neighbors) == 2);

    struct edge_t *edges = gops->getEdges(&u, g);
    ck_assert(edges != NULL);
    ck_assert(countAndFreeEdges(edges) == 2);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * resetGraph clears capacity and flow on array graphs.
 */
START_TEST(arrayResetTest) {
    struct dimensions_t *dims = createDimensions(3, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL);
    ck_assert(dims != NULL);
    struct graph_t *g = initGraph(ARRAY | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t u = 0;
    size_t v = 1;
    double cap = ARRAY_CAP_VAL;
    double flow = ARRAY_FLOW_VAL;
    double got = -1.0;

    ck_assert(gops->addEdge(&u, &v, &cap, g) == GRAPH_OK);
    ck_assert(gops->setFlow(&u, &v, &flow, g) == GRAPH_OK);
    ck_assert(gops->resetGraph(g, NULL, NULL) == GRAPH_OK);

    ck_assert(gops->getCapacity(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == 0.0);
    ck_assert(gops->getFlow(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == 0.0);

    // Edge adjacency remains after reset
    struct edge_t *e = gops->getEdge(&u, &v, g);
    ck_assert(e != NULL);
    free(e);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * Array structure is fixed: addNode is a no-op success; degree slots can fill up.
 */
START_TEST(arrayFixedStructureTest) {
    struct dimensions_t *dims = createDimensions(3, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL);
    ck_assert(dims != NULL);
    struct graph_t *g = initGraph(ARRAY | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t id = 7;
    ck_assert(gops->addNode(&id, g) == GRAPH_OK);
    ck_assert(gops->removeNode(&id, g) == GRAPH_OK);

    size_t u = 0;
    double cap = 1.0;
    // degree == dimcount == 3 for this graph
    size_t v0 = 1, v1 = 2, v2 = 3, v3 = 4;
    ck_assert(gops->addEdge(&u, &v0, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&u, &v1, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&u, &v2, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&u, &v3, &cap, g) == GRAPH_ERR);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * @brief Test basic operation for the link graph structure.
 */
START_TEST(linkGraphTest) {

    struct graph_t *g = initGraph(LINKED | DIRECTED | GENERIC, 0, NULL);
    ck_assert((g->gtype & DIRECTED) == DIRECTED);
    ck_assert((g->gtype & LINKED) == LINKED);
    struct graphops_t *gops = getOperations(g);
    fillLinkTestGraph(g, gops);

    size_t ncount = gops->nodeCount(g);
    ck_assert(LINK_NODE_COUNT == ncount);
    size_t ecount = gops->edgeCount(g);
    ck_assert(ecount == (LINK_NODE_COUNT - 1) * LINK_NODE_COUNT);

    //walk through the edges and get the values
    for (size_t i = 0; i < LINK_NODE_COUNT; i++) {
        for (size_t j = 0; j < LINK_NODE_COUNT; j++) {
            struct edge_t *e = gops->getEdge(&i, &j, g);
            if (i != j) {
                ck_assert(e != NULL);
                ck_assert(e->cap == LINK_CAP_VAL);
                destroyEdges((void **)&e);
            } else {
                ck_assert(e == NULL);
            }
        }
    }

    int opsclear = destroyGraphops((void **) &gops);
    ck_assert(opsclear == GRAPH_OK);
    ck_assert(gops == NULL);
    int graphclear = clearGraph(g);
    ck_assert(graphclear == GRAPH_OK);
    int deallocsuccess = destroyGraph((void **)&g);
    ck_assert(deallocsuccess == GRAPH_OK);
    ck_assert(g == NULL);
}
END_TEST

/**
 * Capacity/flow mutators on a small linked graph.
 */
START_TEST(linkCapFlowTest) {
    struct graph_t *g = initGraph(LINKED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 0, b = 1, c = 2;
    double cap = LINK_CAP_VAL;
    double flow = LINK_FLOW_VAL;
    double got = 0.0;

    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addNode(&c, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);

    ck_assert(gops->getCapacity(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == LINK_CAP_VAL);

    double newcap = 50.0;
    ck_assert(gops->setCapacity(&a, &b, &newcap, g) == GRAPH_OK);
    ck_assert(gops->addCapacity(&a, &b, &flow, g) == GRAPH_OK);
    ck_assert(gops->getCapacity(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == 50.0 + LINK_FLOW_VAL);

    ck_assert(gops->setFlow(&a, &b, &flow, g) == GRAPH_OK);
    ck_assert(gops->addFlow(&a, &b, &flow, g) == GRAPH_OK);
    ck_assert(gops->getFlow(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == LINK_FLOW_VAL * 2.0);

    // Directed: reverse edge does not exist
    ck_assert(gops->getCapacity(&b, &a, &got, g) == GRAPH_ERR);
    // Missing endpoints
    ck_assert(gops->getCapacity(&a, &c, &got, g) == GRAPH_ERR);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * Neighbor/edge lists for linked graphs.
 */
START_TEST(linkNeighborTest) {
    struct graph_t *g = initGraph(LINKED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 0, b = 1, c = 2;
    double cap = LINK_CAP_VAL;
    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addNode(&c, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &c, &cap, g) == GRAPH_OK);

    struct node_t *neighbors = gops->getNeighbors(&a, g);
    ck_assert(neighbors != NULL);
    ck_assert(countAndFreeNodes(neighbors) == 2);

    struct edge_t *edges = gops->getEdges(&a, g);
    ck_assert(edges != NULL);
    ck_assert(countAndFreeEdges(edges) == 2);

    // Node with no outgoing edges
    ck_assert(gops->getNeighbors(&b, g) == NULL);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * removeEdge / removeNode behavior on linked graphs.
 */
START_TEST(linkRemoveTest) {
    struct graph_t *g = initGraph(LINKED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 0, b = 1, c = 2;
    double cap = LINK_CAP_VAL;
    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addNode(&c, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &c, &cap, g) == GRAPH_OK);
    ck_assert(gops->edgeCount(g) == 2);

    ck_assert(gops->removeEdge(&a, &b, g) == GRAPH_OK);
    ck_assert(gops->getEdge(&a, &b, g) == NULL);
    ck_assert(gops->edgeCount(g) == 1);

    ck_assert(gops->removeNode(&c, g) == GRAPH_OK);
    ck_assert(gops->nodeCount(g) == 2);
    ck_assert(gops->getNode(&c, g) == NULL);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * resetGraph zeroes capacity and flow but keeps structure.
 */
START_TEST(linkResetTest) {
    struct graph_t *g = initGraph(LINKED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 0, b = 1;
    double cap = LINK_CAP_VAL;
    double flow = LINK_FLOW_VAL;
    double got = -1.0;

    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);
    ck_assert(gops->setFlow(&a, &b, &flow, g) == GRAPH_OK);
    ck_assert(gops->resetGraph(g, NULL, NULL) == GRAPH_OK);

    ck_assert(gops->getCapacity(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == 0.0);
    ck_assert(gops->getFlow(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == 0.0);
    struct edge_t *kept = gops->getEdge(&a, &b, g);
    ck_assert(kept != NULL);
    destroyEdges((void **)&kept);
    ck_assert(gops->nodeCount(g) == 2);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * Test init and dealloc of shared memory graph
 */
START_TEST(shmemGraphTest) {
    struct dimensions_t *dims = createDimensions(3, ARRAY_DIM_CUBE, ARRAY_DIM_CUBE, ARRAY_DIM_CUBE);
    ck_assert(dims != NULL);
    struct graph_t *g = initGraph(SHARED_MEM | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    ck_assert(g->dims == dims);
    ck_assert((g->gtype & SHARED_MEM) == SHARED_MEM);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);
    size_t ncount = gops->nodeCount(g);
    size_t nexpected = cartesianIndexLength(dims);
    ck_assert(ncount == nexpected);
    int opsclear = destroyGraphops((void **)&gops);
    ck_assert(opsclear == GRAPH_OK);
    ck_assert(gops == NULL);
    int graphclear = clearGraph(g);
    ck_assert(graphclear == GRAPH_OK);
    int deallocsuccess = destroyGraph((void **)&g);
    ck_assert(deallocsuccess == GRAPH_OK);
    ck_assert(g == NULL);
}
END_TEST

/**
 * Test init and dealloc of memory-mapped graph
 */
START_TEST(shmmapGraphTest) {
    struct dimensions_t *dims = createDimensions(3, ARRAY_DIM_CUBE, ARRAY_DIM_CUBE, ARRAY_DIM_CUBE);
    ck_assert(dims != NULL);
    struct graph_t *g = initGraph(SHARED_MMAP | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    ck_assert(g->dims == dims);
    ck_assert((g->gtype & SHARED_MMAP) == SHARED_MMAP);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);
    size_t ncount = gops->nodeCount(g);
    size_t nexpected = cartesianIndexLength(dims);
    ck_assert(ncount == nexpected);
    int opsclear = destroyGraphops((void **)&gops);
    ck_assert(opsclear == GRAPH_OK);
    ck_assert(gops == NULL);
    int graphclear = clearGraph(g);
    ck_assert(graphclear == GRAPH_OK);
    int deallocsuccess = destroyGraph((void **)&g);
    ck_assert(deallocsuccess == GRAPH_OK);
    ck_assert(g == NULL);
}
END_TEST

/**
 * HASHED: init, add/get nodes, grow past initial load, reject duplicates, free.
 */
START_TEST(hashGraphNodeTest) {
    struct graph_t *g = initGraph(HASHED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    ck_assert((g->gtype & HASHED) == HASHED);

    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);
    ck_assert(gops->nodeCount(g) == 0);

    /* Initial table is 17 buckets / grow_at ~12 — insert enough to force a rehash */
    const size_t n = 40;
    for (size_t i = 0; i < n; i++) {
        ck_assert(gops->addNode(&i, g) == GRAPH_OK);
    }
    ck_assert(gops->nodeCount(g) == n);

    for (size_t i = 0; i < n; i++) {
        struct node_t *node = gops->getNode(&i, g);
        ck_assert(node != NULL);
        ck_assert(node->nodeid == i);
        destroyNodes((void **)&node);
    }

    size_t missing = n + 100;
    ck_assert(gops->getNode(&missing, g) == NULL);

    size_t dup = 7;
    ck_assert(gops->addNode(&dup, g) == GRAPH_ERR);
    ck_assert(gops->nodeCount(g) == n);

    ck_assert(destroyGraphops((void **)&gops) == GRAPH_OK);
    ck_assert(clearGraph(g) == GRAPH_OK);
    ck_assert(destroyGraph((void **)&g) == GRAPH_OK);
    ck_assert(g == NULL);
}
END_TEST

/**
 * HASHED directed edges: add/get/list/remove + capacity/flow + duplicate reject.
 */
START_TEST(hashGraphDirectedEdgeTest) {
    struct graph_t *g = initGraph(HASHED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 10, b = 20, c = 30;
    double cap = LINK_CAP_VAL;
    double flow = LINK_FLOW_VAL;
    double got = 0.0;

    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addNode(&c, g) == GRAPH_OK);

    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &c, &cap, g) == GRAPH_OK);
    ck_assert(gops->edgeCount(g) == 2);
    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_ERR);

    struct edge_t *e = gops->getEdge(&a, &b, g);
    ck_assert(e != NULL);
    ck_assert(e->u == a && e->v == b);
    ck_assert(e->cap == LINK_CAP_VAL);
    destroyEdges((void **)&e);
    ck_assert(gops->getEdge(&b, &a, g) == NULL);

    struct edge_t *elist = gops->getEdges(&a, g);
    ck_assert(elist != NULL);
    ck_assert(countAndFreeEdges(elist) == 2);
    ck_assert(gops->getEdges(&b, g) == NULL);

    ck_assert(gops->setFlow(&a, &b, &flow, g) == GRAPH_OK);
    ck_assert(gops->getFlow(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == LINK_FLOW_VAL);
    ck_assert(gops->addCapacity(&a, &b, &flow, g) == GRAPH_OK);
    ck_assert(gops->getCapacity(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == LINK_CAP_VAL + LINK_FLOW_VAL);

    ck_assert(gops->removeEdge(&a, &b, g) == GRAPH_OK);
    ck_assert(gops->getEdge(&a, &b, g) == NULL);
    ck_assert(gops->edgeCount(g) == 1);
    ck_assert(gops->removeEdge(&a, &b, g) == GRAPH_ERR);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * HASHED undirected: both directions stored; either order resolves; remove clears both.
 */
START_TEST(hashGraphUndirectedEdgeTest) {
    struct graph_t *g = initGraph(HASHED | UNDIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    ck_assert(GRAPH_IS_UNDIRECTED(g->gtype));
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 1, b = 2, c = 3;
    double cap = LINK_CAP_VAL;
    double newcap = 99.0;
    double got = 0.0;

    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addNode(&c, g) == GRAPH_OK);

    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);
    ck_assert(gops->edgeCount(g) == 2);
    struct edge_t *ab = gops->getEdge(&a, &b, g);
    struct edge_t *ba = gops->getEdge(&b, &a, g);
    ck_assert(ab != NULL);
    ck_assert(ba != NULL);
    destroyEdges((void **)&ab);
    destroyEdges((void **)&ba);
    ck_assert(gops->addEdge(&b, &a, &cap, g) == GRAPH_ERR);

    ck_assert(gops->setCapacity(&b, &a, &newcap, g) == GRAPH_OK);
    ck_assert(gops->getCapacity(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == newcap);
    ck_assert(gops->getCapacity(&b, &a, &got, g) == GRAPH_OK);
    ck_assert(got == newcap);

    ck_assert(gops->addEdge(&a, &c, &cap, g) == GRAPH_OK);
    ck_assert(gops->edgeCount(g) == 4);

    struct edge_t *from_a = gops->getEdges(&a, g);
    ck_assert(countAndFreeEdges(from_a) == 2);

    ck_assert(gops->removeEdge(&b, &a, g) == GRAPH_OK);
    ck_assert(gops->getEdge(&a, &b, g) == NULL);
    ck_assert(gops->getEdge(&b, &a, g) == NULL);
    ck_assert(gops->edgeCount(g) == 2);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * HASHED neighbors + directed removeNode (incoming and outgoing scrubbed).
 */
START_TEST(hashGraphNeighborRemoveTest) {
    struct graph_t *g = initGraph(HASHED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 0, b = 1, c = 2;
    double cap = LINK_CAP_VAL;
    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addNode(&c, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &c, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&b, &c, &cap, g) == GRAPH_OK);
    ck_assert(gops->edgeCount(g) == 3);

    struct node_t *neighbors = gops->getNeighbors(&a, g);
    ck_assert(neighbors != NULL);
    ck_assert(countAndFreeNodes(neighbors) == 2);
    ck_assert(gops->getNeighbors(&c, g) == NULL);

    ck_assert(gops->removeNode(&c, g) == GRAPH_OK);
    ck_assert(gops->getNode(&c, g) == NULL);
    ck_assert(gops->nodeCount(g) == 2);
    ck_assert(gops->getEdge(&a, &c, g) == NULL);
    ck_assert(gops->getEdge(&b, &c, g) == NULL);
    ck_assert(gops->edgeCount(g) == 1);
    struct edge_t *kept = gops->getEdge(&a, &b, g);
    ck_assert(kept != NULL);
    destroyEdges((void **)&kept);

    ck_assert(gops->removeNode(&c, g) == GRAPH_ERR);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * HASHED undirected removeNode clears both-direction records.
 */
START_TEST(hashGraphUndirectedRemoveTest) {
    struct graph_t *g = initGraph(HASHED | UNDIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 5, b = 6, c = 7;
    double cap = LINK_CAP_VAL;
    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addNode(&c, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &c, &cap, g) == GRAPH_OK);
    ck_assert(gops->edgeCount(g) == 4);

    ck_assert(gops->removeNode(&a, g) == GRAPH_OK);
    ck_assert(gops->nodeCount(g) == 2);
    ck_assert(gops->edgeCount(g) == 0);
    ck_assert(gops->getEdge(&b, &a, g) == NULL);
    ck_assert(gops->getNeighbors(&b, g) == NULL);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

/**
 * HASHED resetGraph zeroes cap/flow but keeps structure.
 */
START_TEST(hashGraphResetTest) {
    struct graph_t *g = initGraph(HASHED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t a = 0, b = 1;
    double cap = LINK_CAP_VAL;
    double flow = LINK_FLOW_VAL;
    double got = -1.0;

    ck_assert(gops->addNode(&a, g) == GRAPH_OK);
    ck_assert(gops->addNode(&b, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&a, &b, &cap, g) == GRAPH_OK);
    ck_assert(gops->setFlow(&a, &b, &flow, g) == GRAPH_OK);
    ck_assert(gops->resetGraph(g, NULL, NULL) == GRAPH_OK);

    ck_assert(gops->getCapacity(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == 0.0);
    ck_assert(gops->getFlow(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == 0.0);
    struct edge_t *kept = gops->getEdge(&a, &b, g);
    ck_assert(kept != NULL);
    destroyEdges((void **)&kept);
    ck_assert(gops->nodeCount(g) == 2);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

#define ENUM_MAX 64

struct idbag {
    size_t ids[ENUM_MAX];
    size_t count;
};

struct stopbag {
    size_t seen;
    size_t limit;
};

static int collectId(const size_t *nodeid, void *ctx) {
    struct idbag *bag = (struct idbag *)ctx;
    if (nodeid == NULL || bag->count >= ENUM_MAX) {
        return GRAPH_ERR;
    }
    bag->ids[bag->count++] = *nodeid;
    return GRAPH_OK;
}

static int bagHasOnce(const struct idbag *bag, size_t id) {
    size_t hits = 0;
    for (size_t i = 0; i < bag->count; i++) {
        if (bag->ids[i] == id) {
            hits++;
        }
    }
    return hits == 1;
}

static int stopAfter(const size_t *nodeid, void *ctx) {
    struct stopbag *stop = (struct stopbag *)ctx;
    (void)nodeid;
    stop->seen++;
    if (stop->seen >= stop->limit) {
        return GRAPH_ERR;
    }
    return GRAPH_OK;
}

/**
 * List and foreach visit exactly the expected ids, and destroying the list
 * leaves the live graph in place.
 */
static void assertEnumMatches(struct graphops_t *gops, struct graph_t *g,
                              const size_t *expected, size_t n) {
    struct node_t *list = NULL;
    struct idbag from_list;
    struct idbag from_each;
    size_t before = 0;

    ck_assert(gops->listNodes != NULL);
    ck_assert(gops->foreachNode != NULL);
    ck_assert(n <= ENUM_MAX);
    ck_assert(gops->nodeCount(g) == n);

    from_list.count = 0;
    from_each.count = 0;
    list = gops->listNodes(g);
    if (n == 0) {
        ck_assert(list == NULL);
    }
    for (struct node_t *curr = list; curr != NULL; curr = curr->next) {
        ck_assert(collectId(&curr->nodeid, &from_list) == GRAPH_OK);
    }
    ck_assert(from_list.count == n);
    for (size_t i = 0; i < n; i++) {
        ck_assert(bagHasOnce(&from_list, expected[i]));
    }

    before = gops->nodeCount(g);
    if (list != NULL) {
        ck_assert(destroyNodes((void **)&list) == GRAPH_OK);
        ck_assert(list == NULL);
    }
    ck_assert(gops->nodeCount(g) == before);
    for (size_t i = 0; i < n; i++) {
        struct node_t *again = gops->getNode(&expected[i], g);
        ck_assert(again != NULL);
        ck_assert(again->nodeid == expected[i]);
        destroyNodes((void **)&again);
    }

    ck_assert(gops->foreachNode(g, collectId, &from_each) == GRAPH_OK);
    ck_assert(from_each.count == n);
    for (size_t i = 0; i < n; i++) {
        ck_assert(bagHasOnce(&from_each, expected[i]));
    }
}

static void assertSnapshotIsolation(struct graphops_t *gops, struct graph_t *g,
                                    size_t a, size_t b) {
    struct node_t *node = gops->getNode(&a, g);
    struct node_t *again = NULL;
    struct edge_t *edge = NULL;
    struct edge_t *still = NULL;
    double got = 0.0;

    ck_assert(node != NULL);
    ck_assert(node->nodeid == a);
    ck_assert(node->edges == NULL);
    ck_assert(destroyNodes((void **)&node) == GRAPH_OK);
    again = gops->getNode(&a, g);
    ck_assert(again != NULL);
    ck_assert(again->nodeid == a);
    destroyNodes((void **)&again);

    edge = gops->getEdge(&a, &b, g);
    ck_assert(edge != NULL);
    edge->cap = -12345.0;
    edge->flow = -99.0;
    destroyEdges((void **)&edge);

    ck_assert(gops->getCapacity(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == LINK_CAP_VAL);
    ck_assert(gops->getFlow(&a, &b, &got, g) == GRAPH_OK);
    ck_assert(got == LINK_FLOW_VAL);

    still = gops->getEdge(&a, &b, g);
    ck_assert(still != NULL);
    ck_assert(still->cap == LINK_CAP_VAL);
    ck_assert(still->flow == LINK_FLOW_VAL);
    destroyEdges((void **)&still);
}

START_TEST(arrayEnumTest) {
    struct dimensions_t *dims = createDimensions(3, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL, ARRAY_DIM_SMALL);
    struct graph_t *g = NULL;
    struct graphops_t *gops = NULL;
    size_t expected[ENUM_MAX];
    size_t n = 0;
    struct stopbag stop;
    struct idbag ignored;

    ck_assert(dims != NULL);
    g = initGraph(ARRAY | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    gops = getOperations(g);
    ck_assert(gops != NULL);

    n = gops->nodeCount(g);
    ck_assert(n > 1);
    ck_assert(n <= ENUM_MAX);
    for (size_t i = 0; i < n; i++) {
        expected[i] = i;
    }
    assertEnumMatches(gops, g, expected, n);

    stop.seen = 0;
    stop.limit = 1;
    ck_assert(gops->foreachNode(g, stopAfter, &stop) == GRAPH_ERR);
    ck_assert(stop.seen == 1);

    ignored.count = 0;
    ck_assert(gops->foreachNode(NULL, collectId, &ignored) == GRAPH_ERR);
    ck_assert(gops->foreachNode(g, NULL, NULL) == GRAPH_ERR);
    ck_assert(gops->listNodes(NULL) == NULL);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

START_TEST(linkEnumAndCopyTest) {
    struct graph_t *g = initGraph(LINKED | DIRECTED | GENERIC, 0, NULL);
    struct graphops_t *gops = NULL;
    size_t ids[] = {2, 9, 100};
    size_t remain[] = {2, 100};
    double cap = LINK_CAP_VAL;
    double flow = LINK_FLOW_VAL;
    struct stopbag stop;

    ck_assert(g != NULL);
    gops = getOperations(g);
    ck_assert(gops != NULL);
    assertEnumMatches(gops, g, ids, 0);

    ck_assert(gops->addNode(&ids[0], g) == GRAPH_OK);
    ck_assert(gops->addNode(&ids[1], g) == GRAPH_OK);
    ck_assert(gops->addNode(&ids[2], g) == GRAPH_OK);
    ck_assert(gops->addEdge(&ids[0], &ids[1], &cap, g) == GRAPH_OK);
    ck_assert(gops->setFlow(&ids[0], &ids[1], &flow, g) == GRAPH_OK);
    assertEnumMatches(gops, g, ids, 3);
    assertSnapshotIsolation(gops, g, ids[0], ids[1]);

    ck_assert(gops->removeNode(&ids[1], g) == GRAPH_OK);
    assertEnumMatches(gops, g, remain, 2);

    stop.seen = 0;
    stop.limit = 1;
    ck_assert(gops->foreachNode(g, stopAfter, &stop) == GRAPH_ERR);
    ck_assert(stop.seen == 1);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

START_TEST(hashEnumAndCopyTest) {
    struct graph_t *g = initGraph(HASHED | DIRECTED | GENERIC, 0, NULL);
    struct graphops_t *gops = NULL;
    size_t sparse[] = {2, 9, 100};
    size_t remain[] = {2, 100};
    size_t dense[40];
    double cap = LINK_CAP_VAL;
    double flow = LINK_FLOW_VAL;

    ck_assert(g != NULL);
    gops = getOperations(g);
    ck_assert(gops != NULL);
    assertEnumMatches(gops, g, sparse, 0);

    ck_assert(gops->addNode(&sparse[0], g) == GRAPH_OK);
    ck_assert(gops->addNode(&sparse[1], g) == GRAPH_OK);
    ck_assert(gops->addNode(&sparse[2], g) == GRAPH_OK);
    ck_assert(gops->addEdge(&sparse[0], &sparse[1], &cap, g) == GRAPH_OK);
    ck_assert(gops->setFlow(&sparse[0], &sparse[1], &flow, g) == GRAPH_OK);
    assertEnumMatches(gops, g, sparse, 3);
    assertSnapshotIsolation(gops, g, sparse[0], sparse[1]);

    ck_assert(gops->removeNode(&sparse[1], g) == GRAPH_OK);
    assertEnumMatches(gops, g, remain, 2);

    clearGraph(g);
    destroyGraph((void **)&g);
    destroyGraphops((void **)&gops);

    g = initGraph(HASHED | DIRECTED | GENERIC, 0, NULL);
    ck_assert(g != NULL);
    gops = getOperations(g);
    ck_assert(gops != NULL);
    for (size_t i = 0; i < 40; i++) {
        dense[i] = i;
        ck_assert(gops->addNode(&dense[i], g) == GRAPH_OK);
    }
    assertEnumMatches(gops, g, dense, 40);

    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}
END_TEST

static void assertDenseSharedEnum(enum GRAPHDOMAIN impl) {
    struct dimensions_t *dims = createDimensions(3, 2, 2, 2);
    struct graph_t *g = NULL;
    struct graph_t *ro = NULL;
    struct graphops_t *gops = NULL;
    struct graphops_t *rops = NULL;
    size_t expected[ENUM_MAX];
    size_t n = 0;

    ck_assert(dims != NULL);
    g = initGraph(impl | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    gops = getOperations(g);
    ck_assert(gops != NULL);
    n = gops->nodeCount(g);
    ck_assert(n > 0);
    ck_assert(n <= ENUM_MAX);
    for (size_t i = 0; i < n; i++) {
        expected[i] = i;
    }
    assertEnumMatches(gops, g, expected, n);

    ro = attachReadonlyGraph(g);
    ck_assert(ro != NULL);
    rops = getOperations(ro);
    ck_assert(rops != NULL);
    assertEnumMatches(rops, ro, expected, n);

    destroyGraphops((void **)&rops);
    clearGraph(ro);
    destroyGraph((void **)&ro);
    destroyGraphops((void **)&gops);
    clearGraph(g);
    destroyGraph((void **)&g);
}

START_TEST(shmemEnumTest) {
    assertDenseSharedEnum(SHARED_MEM);
}
END_TEST

START_TEST(shmmapEnumTest) {
    assertDenseSharedEnum(SHARED_MMAP);
}
END_TEST

Suite * init_suite(void) {
    Suite * s;
    TCase *tc_core;

    s = suite_create("Graph");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, arrayGraphTest);
    tcase_add_test(tc_core, arrayCapFlowTest);
    tcase_add_test(tc_core, arrayNeighborTest);
    tcase_add_test(tc_core, arrayResetTest);
    tcase_add_test(tc_core, arrayFixedStructureTest);
    tcase_add_test(tc_core, linkGraphTest);
    tcase_add_test(tc_core, linkCapFlowTest);
    tcase_add_test(tc_core, linkNeighborTest);
    tcase_add_test(tc_core, linkRemoveTest);
    tcase_add_test(tc_core, linkResetTest);
    tcase_add_test(tc_core, hashGraphNodeTest);
    tcase_add_test(tc_core, hashGraphDirectedEdgeTest);
    tcase_add_test(tc_core, hashGraphUndirectedEdgeTest);
    tcase_add_test(tc_core, hashGraphNeighborRemoveTest);
    tcase_add_test(tc_core, hashGraphUndirectedRemoveTest);
    tcase_add_test(tc_core, hashGraphResetTest);
    tcase_add_test(tc_core, shmemGraphTest);
    tcase_add_test(tc_core, shmmapGraphTest);
    tcase_add_test(tc_core, arrayEnumTest);
    tcase_add_test(tc_core, linkEnumAndCopyTest);
    tcase_add_test(tc_core, hashEnumAndCopyTest);
    tcase_add_test(tc_core, shmemEnumTest);
    tcase_add_test(tc_core, shmmapEnumTest);
    suite_add_tcase(s, tc_core);

    return s;
}


int main(void) {
    int number_failed;
    Suite * s;
    SRunner *sr;

    s = init_suite();
    sr = srunner_create(s);
    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
