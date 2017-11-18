//
// Created by david on 11/18/17.
//

#include <CUnit/Basic.h>
#include <graphdata.h>
#include <util/spatial.h>
#include <graphinit.h>
#include <stdlib.h>

static void checkArrays(size_t *expected, size_t *calced, size_t len) {
    CU_ASSERT(expected != NULL);
    CU_ASSERT(calced != NULL);
    for (int i=0;i<len;i++) {
        CU_ASSERT(expected[i] == calced[i]);
    }
}

/**
 * @brief Test that we are getting correct index lengths
 */
void indexLengthTest(void) {
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    CU_ASSERT(NULL != dim1);
    size_t len1 = indexLength(dim1);
    CU_ASSERT(len1 == 1000000);
    free(dim1);

    struct dimensions_t *dim2 = createDimensions(2,10,10);
    CU_ASSERT(NULL != dim2);
    size_t len2 = indexLength(dim2);
    CU_ASSERT(len2 == 100);

    struct dimensions_t *dim3 = createDimensions(2, 256,256);
    CU_ASSERT(NULL != dim3);
    size_t len3 = indexLength(dim3);
    CU_ASSERT(len3 == 65536)
    free(dim3);
}

/**
 * @brief Test that we are getting valid coordinates
 */
void coordTest(void) {


    size_t coord3d[3] = { 5, 55, 32 };
    size_t coord2d[2] = {77, 222};
    size_t idx1 = 325505;
    size_t calc1[3] = {0,0,0};
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    CU_ASSERT(NULL != dim1);
    int ret1 = spatialFromIndex(&idx1, calc1, dim1);
    CU_ASSERT( ret1 == 0);
    checkArrays(coord3d, calc1, dim1->dimcount);
    free(dim1);

    size_t idx2 = 2111237;
    size_t calc2[3] = {0,0,0};
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    CU_ASSERT(NULL != dim2);
    int ret2 = spatialFromIndex(&idx2, calc2, dim2);
    CU_ASSERT(ret2 == 0);
    checkArrays(coord3d, calc2, dim2->dimcount);
    free(dim2);

    size_t idx3 = 56909;
    size_t calc3[2] = {0,0};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    CU_ASSERT(NULL != dim3);
    int ret3 = spatialFromIndex(&idx3, calc3, dim3);
    CU_ASSERT(ret3 == 0);
    checkArrays(coord2d, calc3, dim3->dimcount);
    free(dim3);

    

}

/**
 * @brief Test that we are detecting invalid coordinates
 */
void coordFailTest(void) {

}

/**
 * @brief Test that we are getting valid index values from coordinates
 */
void indexTest(void) {

}

/**
 * @brief Test that we are detecting invalid index values from coordinates.
 */
void indexFailTest(void) {

}

int main (int argc, char** argv) {
    CU_pSuite  pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Spatial Suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Index lengths", indexLengthTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Good coordinates", coordTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    if (NULL == CU_add_test(pSuite, "Bad coordinates", coordFailTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    if (NULL == CU_add_test(pSuite, "Good indexes", indexTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    if (NULL == CU_add_test(pSuite, "Bad indexes", indexFailTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
