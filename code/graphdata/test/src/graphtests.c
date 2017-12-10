//
// Created by david on 12/3/17.
//


#include <CUnit/Basic.h>
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
    for (size_t u = 0;u<ncount-1;u++) {
        for (size_t v = 0;v<ncount-1;v++) {
            gops->setCapacity(&u,&v,&cap, g);
        }
    }
}

/**
 * Fill linked graph according to LINK_EDGE_COUNT and LINK_NODE_COUNT
 * @param g Link graph to be modified.
 */
void fillLinkTestGraph(struct graph_t *g, struct graphops_t *gops) {

    for (size_t i=0;i<LINK_NODE_COUNT;i++) {
        gops->addNode(&i, g);
    }
    double cap = LINK_CAP_VAL;
    for (size_t u=0;u<LINK_NODE_COUNT;u++) {
        for (size_t v=0;v<LINK_NODE_COUNT;v++) {
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
void arrayGraphTest(void) {

    struct dimensions_t *dims = createDimensions(3,ARRAY_DIM_CUBE, ARRAY_DIM_CUBE, ARRAY_DIM_CUBE);
    CU_ASSERT(dims != NULL);
    struct graph_t *g = initGraph(ARRAY | UNDIRECTED | GENERIC, 0, dims);
    CU_ASSERT(g != NULL);
    CU_ASSERT(g->dims == dims);
    struct graphops_t *gops = getOperations(g);
    CU_ASSERT(gops != NULL);
    size_t ncount = gops->nodeCount(g);
    size_t nexpected = cartesianIndexLength(dims);
    CU_ASSERT(ncount == nexpected);
    int opsclear = destroyGraphops((void **)&gops);
    CU_ASSERT(opsclear == 1);
    CU_ASSERT(gops == NULL);
    int graphclear = clearGraph(g);
    CU_ASSERT(graphclear == 1);
    int deallocsuccess = destroyGraph((void **)&g);
    CU_ASSERT(deallocsuccess == 1);
    CU_ASSERT(g == NULL);

}

/**
 * @brief Test basic operation for the link graph structure.
 */
void linkGraphTest(void) {

    struct graph_t *g = initGraph(LINKED | DIRECTED | GENERIC, 0, NULL);
    CU_ASSERT((g->gtype & DIRECTED) == DIRECTED);
    CU_ASSERT((g->gtype & LINKED) == LINKED);
    struct graphops_t *gops = getOperations(g);
    fillLinkTestGraph(g, gops);

    size_t ncount = gops->nodeCount(g);
    CU_ASSERT(LINK_NODE_COUNT == ncount);
    size_t ecount = gops->edgeCount(g);
    CU_ASSERT(ecount == (LINK_NODE_COUNT -1)*LINK_NODE_COUNT);

    //walk through the edges and get the values
    for (size_t i=0;i<LINK_NODE_COUNT;i++) {
        for (size_t j=0;j<LINK_NODE_COUNT;j++) {
            struct edge_t *e = gops->getEdge(&i, &j, g);
            if (i != j) {
                CU_ASSERT(e != NULL);
                CU_ASSERT(e->cap = LINK_CAP_VAL);
            } else {
                CU_ASSERT(e == NULL);
            }
        }
    }

    int opsclear = destroyGraphops((void **)&gops);
    CU_ASSERT(opsclear == 1);
    CU_ASSERT(gops == NULL);
    int graphclear = clearGraph(g);
    CU_ASSERT(graphclear == 1);
    int deallocsuccess = destroyGraph((void **)&g);
    CU_ASSERT(deallocsuccess == 1);
    CU_ASSERT(g == NULL);
}

int main (int argc, char** argv) {
    CU_pSuite  pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Graph operations Suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Array Graph", arrayGraphTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Link Graph", linkGraphTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
