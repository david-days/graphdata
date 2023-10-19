//
// Created by david on 12/3/17.
//


#include <check.h>
#include <graphinit.h>
#include <graphops.h>
#include <util/crudops.h>
#include <stdlib.h>
#include <util/cartesian.h>


#define ARRAY_DIM_CUBE 10
#define ARRAY_CAP_VAL 55.0
#define LINK_CAP_VAL 37.0
#define LINK_NODE_COUNT 6


/**
 * Fill array graph according to ARRAY_EDGE_COUNT and ARRAY_NODE_COUNT
 *
 * @param g Array graph structure to be modified
 */
void fillArrayTestGraph(struct graph_t *g, struct graphops_t *gops) {
    //array graphs aren't modifiable, so just set capacity values
    size_t ncount = gops->nodeCount(g);
    double cap = ARRAY_CAP_VAL;
    for (size_t u = 0; u < ncount - 1; u++) {
        for (size_t v = 0; v < ncount - 1; v++) {
            gops->setCapacity(&u, &v, &cap, g);
        }
    }
}

/**
 * Fill linked graph according to LINK_EDGE_COUNT and LINK_NODE_COUNT
 * @param g Link graph to be modified.
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
 * @brief Test basic operations for the array graph structure
 *
 *
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
        ck_assert(opsclear == 1);
        ck_assert(gops == NULL);
        int graphclear = clearGraph(g);
        ck_assert(graphclear == 1);
        int deallocsuccess = destroyGraph((void **)&g);
        ck_assert(deallocsuccess == 1);
        ck_assert(g == NULL);

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
                ck_assert(e->cap = LINK_CAP_VAL);
            } else {
                ck_assert(e == NULL);
            }
        }
    }

    int opsclear = destroyGraphops((void **) &gops);
    ck_assert(opsclear == 1);
    ck_assert(gops == NULL);
    int graphclear = clearGraph(g);
    ck_assert(graphclear == 1);
    int deallocsuccess = destroyGraph((void **) &g);
    ck_assert(deallocsuccess == 1);
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
    tcase_add_test(tc_core, linkGraphTest);
    suite_add_tcase(s, tc_core);

    return s;
}


int main(void) {
    int number_failed;
    Suite * s;
    SRunner *sr;

    s = init_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
