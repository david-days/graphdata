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
        ck_assert(opsclear == EXIT_SUCCESS);
        ck_assert(gops == NULL);
        int graphclear = clearGraph(g);
        ck_assert(graphclear == EXIT_SUCCESS);
        int deallocsuccess = destroyGraph((void **)&g);
        ck_assert(deallocsuccess == EXIT_SUCCESS);
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
            } else {
                ck_assert(e == NULL);
            }
        }
    }

    int opsclear = destroyGraphops((void **) &gops);
    ck_assert(opsclear == EXIT_SUCCESS);
    ck_assert(gops == NULL);
    int graphclear = clearGraph(g);
    ck_assert(graphclear == EXIT_SUCCESS);
    int deallocsuccess = destroyGraph((void **)&g);
    ck_assert(deallocsuccess == EXIT_SUCCESS);
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
    ck_assert(gops->getEdge(&a, &b, g) != NULL);
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
    ck_assert(opsclear == EXIT_SUCCESS);
    ck_assert(gops == NULL);
    int graphclear = clearGraph(g);
    ck_assert(graphclear == EXIT_SUCCESS);
    int deallocsuccess = destroyGraph((void **)&g);
    ck_assert(deallocsuccess == EXIT_SUCCESS);
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
    ck_assert(opsclear == EXIT_SUCCESS);
    ck_assert(gops == NULL);
    int graphclear = clearGraph(g);
    ck_assert(graphclear == EXIT_SUCCESS);
    int deallocsuccess = destroyGraph((void **)&g);
    ck_assert(deallocsuccess == EXIT_SUCCESS);
    ck_assert(g == NULL);
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
    tcase_add_test(tc_core, shmemGraphTest);
    tcase_add_test(tc_core, shmmapGraphTest);
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
