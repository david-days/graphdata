//
// Created by david on 11/18/17.
//

#include <CUnit/Basic.h>
#include <graphinit.h>
#include <stdlib.h>

/**
 * @brief Test the createDimensions() function returns expected structure
 *
 *
 */
void dimTest(void) {
    size_t t1size = 3;
    size_t t1arr[3] = {1,2,3};
    size_t t2size = 5;
    size_t t2arr[5] = {10,20,30,40,50};
    size_t t3size = 2;
    size_t t3arr[2] = {255,255};

    //Start testing
    struct dimensions_t *dim1 = createDimensions(3,1,2,3);
    CU_ASSERT(NULL != dim1);
    CU_ASSERT(dim1->dimcount == t1size);
    for (int i=0;i<t1size;i++) {
        CU_ASSERT(dim1->dimarr[i] == t1arr[i]);
    }
    free(dim1);

    struct dimensions_t *dim2 = createDimensions(5, 10,20,30,40,50);
    CU_ASSERT(NULL != dim2);
    CU_ASSERT(dim2->dimcount == t2size);
    for (int i=0;i<t2size;i++) {
        CU_ASSERT(dim2->dimarr[i] == t2arr[i]);
    }
    free(dim2);

    struct dimensions_t *dim3 = createDimensions(2, 255,255);
    CU_ASSERT(NULL != dim3);
    CU_ASSERT(dim3->dimcount == t3size);
    for (int i=0;i<t3size;i++) {
        CU_ASSERT(dim3->dimarr[i] == t3arr[i]);
    }
    free(dim3);
}

/**
 * Test that the graphops initialization has been properly carried out.
 */
void rawGraphopsTest(void) {
    struct graphops_t *gops = initGraphops();
    CU_ASSERT(NULL != gops);
    CU_ASSERT(NULL == gops->g);
    CU_ASSERT(NULL == gops->addEdge);
    CU_ASSERT(NULL == gops->addNode);
    CU_ASSERT(NULL == gops->edgeCount);
    CU_ASSERT(NULL == gops->edgePath);
    CU_ASSERT(NULL == gops->edgePath);
    CU_ASSERT(NULL == gops->getEdge);
    CU_ASSERT(NULL == gops->getEdges);
    CU_ASSERT(NULL == gops->getNeighbors);
    CU_ASSERT(NULL == gops->getNode);
    CU_ASSERT(NULL == gops->nodeCount);
    CU_ASSERT(NULL == gops->nodePath);
    CU_ASSERT(NULL == gops->resetGraph);
    CU_ASSERT(NULL == gops->setCapacity);
    free(gops);
}

int main (int argc, char** argv) {
    CU_pSuite  pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Initialization Suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Dimension structure", dimTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Graphops init", rawGraphopsTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}