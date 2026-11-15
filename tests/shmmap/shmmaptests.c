//
// Created by david on 8/21/25.
//

#include <check.h>
#include <graphInit.h>
#include <graphOps.h>
#include <util/cartesian.h>
#include <util/crudops.h>

#define SHMMAP_DIM 3
#define SHMMAP_CAP 42.0
#define SHMMAP_FLOW 7.5

static struct graph_t * makeWritableShmmap(struct dimensions_t **dims_out) {
    struct dimensions_t *dims = createDimensions(3, SHMMAP_DIM, SHMMAP_DIM, SHMMAP_DIM);
    ck_assert(dims != NULL);
    struct graph_t *g = initGraph(SHARED_MMAP | UNDIRECTED | GENERIC, 0, dims);
    ck_assert(g != NULL);
    *dims_out = dims;
    return g;
}

static void teardownGraph(struct graph_t **g, struct dimensions_t **dims) {
    if (g != NULL && *g != NULL) {
        clearGraph(*g);
        destroyGraph((void **)g);
    }
    if (dims != NULL && *dims != NULL) {
        destroyDimensions((void **)dims);
    }
}

START_TEST(shmmapInitTeardownTest) {
    struct dimensions_t *dims = NULL;
    struct graph_t *g = makeWritableShmmap(&dims);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);
    ck_assert(gops->nodeCount(g) == cartesianIndexLength(dims));
    destroyGraphops((void **)&gops);
    teardownGraph(&g, &dims);
}
END_TEST

START_TEST(shmmapCapFlowTest) {
    struct dimensions_t *dims = NULL;
    struct graph_t *g = makeWritableShmmap(&dims);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t u = 0, v = 1, missing = 5;
    double cap = SHMMAP_CAP;
    double flow = SHMMAP_FLOW;
    double got = 0.0;

    ck_assert(gops->addEdge(&u, &v, &cap, g) == GRAPH_OK);
    ck_assert(gops->getCapacity(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == SHMMAP_CAP);

    double newcap = 100.0;
    ck_assert(gops->setCapacity(&u, &v, &newcap, g) == GRAPH_OK);
    ck_assert(gops->addCapacity(&u, &v, &flow, g) == GRAPH_OK);
    ck_assert(gops->getCapacity(&v, &u, &got, g) == GRAPH_OK);
    ck_assert(got == 100.0 + SHMMAP_FLOW);

    ck_assert(gops->setFlow(&u, &v, &flow, g) == GRAPH_OK);
    ck_assert(gops->addFlow(&u, &v, &flow, g) == GRAPH_OK);
    ck_assert(gops->getFlow(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == SHMMAP_FLOW * 2.0);

    ck_assert(gops->getCapacity(&u, &missing, &got, g) == GRAPH_ERR);

    destroyGraphops((void **)&gops);
    teardownGraph(&g, &dims);
}
END_TEST

/**
 * Provisional RO accessor: writes must fail; reads still work.
 * Official multi-process reader/discovery API is deferred.
 */
START_TEST(shmmapReadonlyRejectTest) {
    struct dimensions_t *dims = NULL;
    struct graph_t *g = makeWritableShmmap(&dims);
    struct graphops_t *wops = getOperations(g);
    ck_assert(wops != NULL);

    size_t u = 0, v = 1;
    double cap = SHMMAP_CAP;
    double flow = SHMMAP_FLOW;
    double got = 0.0;
    ck_assert(wops->addEdge(&u, &v, &cap, g) == GRAPH_OK);
    ck_assert(wops->setFlow(&u, &v, &flow, g) == GRAPH_OK);

    struct graph_t *ro = attachReadonlyGraph(g);
    ck_assert(ro != NULL);
    ck_assert((ro->gtype & RD_ONLY) == RD_ONLY);
    struct graphops_t *rops = getOperations(ro);
    ck_assert(rops != NULL);

    ck_assert(rops->getCapacity(&u, &v, &got, ro) == GRAPH_OK);
    ck_assert(got == SHMMAP_CAP);
    ck_assert(rops->getFlow(&u, &v, &got, ro) == GRAPH_OK);
    ck_assert(got == SHMMAP_FLOW);

    double attempt = 999.0;
    ck_assert(rops->setCapacity(&u, &v, &attempt, ro) == GRAPH_ERR);
    ck_assert(rops->addCapacity(&u, &v, &attempt, ro) == GRAPH_ERR);
    ck_assert(rops->setFlow(&u, &v, &attempt, ro) == GRAPH_ERR);
    ck_assert(rops->addFlow(&u, &v, &attempt, ro) == GRAPH_ERR);
    ck_assert(rops->addEdge(&u, &v, &attempt, ro) == GRAPH_ERR);
    ck_assert(rops->resetGraph(ro, NULL, NULL) == GRAPH_ERR);

    ck_assert(wops->getCapacity(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == SHMMAP_CAP);

    destroyGraphops((void **)&rops);
    clearGraph(ro);
    destroyGraph((void **)&ro);
    destroyGraphops((void **)&wops);
    teardownGraph(&g, &dims);
}
END_TEST

START_TEST(shmmapResetTest) {
    struct dimensions_t *dims = NULL;
    struct graph_t *g = makeWritableShmmap(&dims);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t u = 0, v = 1;
    double cap = SHMMAP_CAP;
    double flow = SHMMAP_FLOW;
    double got = -1.0;
    ck_assert(gops->addEdge(&u, &v, &cap, g) == GRAPH_OK);
    ck_assert(gops->setFlow(&u, &v, &flow, g) == GRAPH_OK);
    ck_assert(gops->resetGraph(g, NULL, NULL) == GRAPH_OK);
    ck_assert(gops->getCapacity(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == 0.0);
    ck_assert(gops->getFlow(&u, &v, &got, g) == GRAPH_OK);
    ck_assert(got == 0.0);

    destroyGraphops((void **)&gops);
    teardownGraph(&g, &dims);
}
END_TEST

START_TEST(shmmapDegreeExhaustTest) {
    struct dimensions_t *dims = NULL;
    struct graph_t *g = makeWritableShmmap(&dims);
    struct graphops_t *gops = getOperations(g);
    ck_assert(gops != NULL);

    size_t u = 0;
    double cap = 1.0;
    size_t v0 = 1, v1 = 2, v2 = 3, v3 = 4;
    ck_assert(gops->addEdge(&u, &v0, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&u, &v1, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&u, &v2, &cap, g) == GRAPH_OK);
    ck_assert(gops->addEdge(&u, &v3, &cap, g) == GRAPH_ERR);

    destroyGraphops((void **)&gops);
    teardownGraph(&g, &dims);
}
END_TEST

Suite * init_suite(void) {
    Suite * s;
    TCase *tc_core;

    s = suite_create("ShMMap");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, shmmapInitTeardownTest);
    tcase_add_test(tc_core, shmmapCapFlowTest);
    tcase_add_test(tc_core, shmmapReadonlyRejectTest);
    tcase_add_test(tc_core, shmmapResetTest);
    tcase_add_test(tc_core, shmmapDegreeExhaustTest);

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
