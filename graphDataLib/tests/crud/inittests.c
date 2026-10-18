//
// Created by david on 11/18/17.
//

#include <stdlib.h>
#include <check.h>
#include <graphInit.h>
#include <util/crudops.h>

/**
 * Test that the graphops initialization has been properly carried out.
 */
START_TEST(rawGraphopsTest) {
    struct graphops_t *gops = initGraphops();
    ck_assert(NULL != gops);
    ck_assert(NULL == gops->g);
    ck_assert(NULL == gops->addEdge);
    ck_assert(NULL == gops->addNode);
    ck_assert(NULL == gops->edgeCount);
    ck_assert(NULL == gops->getEdge);
    ck_assert(NULL == gops->getEdges);
    ck_assert(NULL == gops->getNeighbors);
    ck_assert(NULL == gops->getNode);
    ck_assert(NULL == gops->nodeCount);
    ck_assert(NULL == gops->resetGraph);
    ck_assert(NULL == gops->setCapacity);
    ck_assert(NULL == gops->listNodes);
    ck_assert(NULL == gops->foreachNode);
    ck_assert(destroyGraphops((void **)&gops) == EXIT_SUCCESS);
    ck_assert(gops == NULL);
}
END_TEST

/**
 * @brief Test the createDimensions() function returns expected structure
 */
START_TEST(dimTest)
        {
            size_t t1size = 3;
            size_t t1arr[3] = {1,2,3};
            size_t t2size = 5;
            size_t t2arr[5] = {10,20,30,40,50};
            size_t t3size = 2;
            size_t t3arr[2] = {255,255};

            struct dimensions_t *dim1 = createDimensions(3,1,2,3);
            ck_assert(NULL != dim1);
            ck_assert(dim1->dimcount == t1size);
            for (int i=0;i<t1size;i++) {
                ck_assert(dim1->dimarr[i] == t1arr[i]);
            }
            ck_assert(destroyDimensions((void **)&dim1) == EXIT_SUCCESS);
            ck_assert(dim1 == NULL);

            struct dimensions_t *dim2 = createDimensions(5, 10,20,30,40,50);
            ck_assert(NULL != dim2);
            ck_assert(dim2->dimcount == t2size);
            for (int i=0;i<t2size;i++) {
                ck_assert(dim2->dimarr[i] == t2arr[i]);
            }
            ck_assert(destroyDimensions((void **)&dim2) == EXIT_SUCCESS);
            ck_assert(dim2 == NULL);

            struct dimensions_t *dim3 = createDimensions(2, 255,255);
            ck_assert(NULL != dim3);
            ck_assert(dim3->dimcount == t3size);
            for (int i=0;i<t3size;i++) {
                ck_assert(dim3->dimarr[i] == t3arr[i]);
            }
            ck_assert(destroyDimensions((void **)&dim3) == EXIT_SUCCESS);
            ck_assert(dim3 == NULL);
        }
END_TEST

START_TEST(destroyDimensionsTest) {
    struct dimensions_t *dims = createDimensions(2, 4, 8);
    ck_assert(dims != NULL);
    ck_assert(dims->dimarr != NULL);
    ck_assert(destroyDimensions((void **)&dims) == EXIT_SUCCESS);
    ck_assert(dims == NULL);
    ck_assert(destroyDimensions((void **)&dims) == EXIT_FAILURE);
}
END_TEST

START_TEST(parseTypeFlagsTest) {
    enum GRAPHDOMAIN flags;
    enum GRAPHDOMAIN dir, imp, lbl, dom, ro;

    flags = 0;
    ck_assert(parseTypeFlags(&flags, &dir, &imp, &lbl, &dom, &ro) == GRAPH_OK);
    ck_assert(dir == UNDIRECTED);
    ck_assert(imp == LINKED);
    ck_assert(lbl == UNLABELED);
    ck_assert(dom == GENERIC);
    ck_assert(ro == 0);
    ck_assert((flags & LINKED) == LINKED);
    ck_assert((flags & GENERIC) == GENERIC);

    flags = ARRAY | DIRECTED | SPATIAL | LABELED;
    ck_assert(parseTypeFlags(&flags, &dir, &imp, &lbl, &dom, &ro) == GRAPH_OK);
    ck_assert(dir == DIRECTED);
    ck_assert(imp == ARRAY);
    ck_assert(lbl == LABELED);
    ck_assert(dom == SPATIAL);
    ck_assert(ro == 0);
    ck_assert((flags & ARRAY) == ARRAY);
    ck_assert((flags & DIRECTED) == DIRECTED);
    ck_assert((flags & SPATIAL) == SPATIAL);
    ck_assert((flags & LABELED) == LABELED);

    /* Conflicting implementation bits must fail; outputs unchanged */
    flags = SHARED_MEM | SHARED_MMAP | UNDIRECTED;
    enum GRAPHDOMAIN before = flags;
    ck_assert(parseTypeFlags(&flags, &dir, &imp, &lbl, &dom, &ro) == GRAPH_ERR);
    ck_assert(flags == before);

    flags = ARRAY | LINKED;
    before = flags;
    ck_assert(parseTypeFlags(&flags, &dir, &imp, &lbl, &dom, &ro) == GRAPH_ERR);
    ck_assert(flags == before);

    flags = LABELED | UNLABELED;
    before = flags;
    ck_assert(parseTypeFlags(&flags, &dir, &imp, &lbl, &dom, &ro) == GRAPH_ERR);
    ck_assert(flags == before);

    flags = GENERIC | SPATIAL | LINKED;
    before = flags;
    ck_assert(parseTypeFlags(&flags, &dir, &imp, &lbl, &dom, &ro) == GRAPH_ERR);
    ck_assert(flags == before);

    ck_assert(parseTypeFlags(NULL, &dir, &imp, &lbl, &dom, &ro) == GRAPH_ERR);
}
END_TEST

/**
 * initGraph rejects nonsense flag/argument combinations.
 */
START_TEST(initGraphRejectTest) {
    struct dimensions_t *dims = createDimensions(2, 3, 3);
    ck_assert(dims != NULL);

    /* Shared backends require dimensions */
    ck_assert(initGraph(SHARED_MEM | UNDIRECTED | GENERIC, 0, NULL) == NULL);
    ck_assert(initGraph(SHARED_MMAP | UNDIRECTED | GENERIC, 0, NULL) == NULL);

    /* RD_ONLY only valid for shared backends */
    ck_assert(initGraph(ARRAY | UNDIRECTED | GENERIC | RD_ONLY, 0, dims) == NULL);
    ck_assert(initGraph(LINKED | DIRECTED | GENERIC | RD_ONLY, 0, NULL) == NULL);

    /* LABELED requires a positive label count */
    ck_assert(initGraph(LINKED | LABELED | GENERIC, 0, NULL) == NULL);

    /* Conflicting impl flags fail parse → NULL */
    ck_assert(initGraph(ARRAY | LINKED | GENERIC, 0, dims) == NULL);

    destroyDimensions((void **)&dims);
}
END_TEST

START_TEST(labelLifecycleTest) {
    struct labels_t *labels = initLabels(4);
    ck_assert(labels != NULL);
    ck_assert(labels->labelcount == 4);
    ck_assert(labels->labelarr != NULL);
    ck_assert(destroyLabels((void **)&labels) == EXIT_SUCCESS);
    ck_assert(labels == NULL);
}
END_TEST

START_TEST(edgeNodeFeatureLifecycleTest) {
    struct node_t *n1 = initNode();
    struct node_t *n2 = initNode();
    ck_assert(n1 != NULL && n2 != NULL);
    n1->nodeid = 10;
    n2->nodeid = 20;
    n1->next = n2;
    n2->prev = n1;

    struct node_t *nc = cloneNode(n1);
    ck_assert(nc != NULL);
    ck_assert(nc->nodeid == 10);
    ck_assert(nc->next == NULL);
    destroyNodes((void **)&nc);

    struct edge_t *e1 = initEdge();
    struct edge_t *e2 = initEdge();
    ck_assert(e1 != NULL && e2 != NULL);
    e1->u = 1;
    e1->v = 2;
    e1->cap = 3.5;
    e1->flow = 1.0;
    e2->u = 2;
    e2->v = 3;
    e2->cap = 9.0;
    e1->next = e2;
    e2->prev = e1;

    struct edge_t *ec = cloneEdge(e1);
    ck_assert(ec != NULL);
    ck_assert(ec->u == 1 && ec->v == 2);
    ck_assert(ec->cap == 3.5 && ec->flow == 1.0);
    ck_assert(ec->next == NULL);
    destroyEdges((void **)&ec);

    struct feature_t *f1 = initFeature();
    ck_assert(f1 != NULL);
    f1->hashid = 123;
    f1->val = 4.25;
    struct feature_t *fc = cloneFeature(f1);
    ck_assert(fc != NULL);
    ck_assert(fc->hashid == 123);
    ck_assert(fc->val == 4.25);

    destroyFeatures((void **)&fc);
    destroyFeatures((void **)&f1);
    destroyEdges((void **)&e1);
    destroyNodes((void **)&n1);
}
END_TEST


Suite * init_suite(void)
{
    Suite *s;
    TCase *tc_core;
    
    s = suite_create("Init");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, rawGraphopsTest);
    tcase_add_test(tc_core, dimTest);
    tcase_add_test(tc_core, destroyDimensionsTest);
    tcase_add_test(tc_core, parseTypeFlagsTest);
    tcase_add_test(tc_core, initGraphRejectTest);
    tcase_add_test(tc_core, labelLifecycleTest);
    tcase_add_test(tc_core, edgeNodeFeatureLifecycleTest);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = init_suite();
    sr = srunner_create(s);
    srunner_set_fork_status(sr, CK_NOFORK);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
