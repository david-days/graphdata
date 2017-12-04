//
// Created by david on 11/18/17.
//

#include <CUnit/Basic.h>
#include <graphdata.h>
#include <util/cartesian.h>
#include <util/crudops.h>


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
    size_t len1 = cartesianIndexLength(dim1);
    CU_ASSERT(len1 == 1000000);
    free(dim1);

    struct dimensions_t *dim2 = createDimensions(2,10,10);
    CU_ASSERT(NULL != dim2);
    size_t len2 = cartesianIndexLength(dim2);
    CU_ASSERT(len2 == 100);
    free(dim2);

    struct dimensions_t *dim3 = createDimensions(2, 256,256);
    CU_ASSERT(NULL != dim3);
    size_t len3 = cartesianIndexLength(dim3);
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
    int ret1 = cartesianFromIndex(&idx1, calc1, dim1);
    CU_ASSERT( ret1 == 0);
    checkArrays(coord3d, calc1, dim1->dimcount);
    free(dim1);

    size_t idx2 = 2111237;
    size_t calc2[3] = {0,0,0};
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    CU_ASSERT(NULL != dim2);
    int ret2 = cartesianFromIndex(&idx2, calc2, dim2);
    CU_ASSERT(ret2 == 0);
    checkArrays(coord3d, calc2, dim2->dimcount);
    free(dim2);

    size_t idx3 = 56909;
    size_t calc3[2] = {0,0};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    CU_ASSERT(NULL != dim3);
    int ret3 = cartesianFromIndex(&idx3, calc3, dim3);
    CU_ASSERT(ret3 == 0);
    checkArrays(coord2d, calc3, dim3->dimcount);
    free(dim3);



}

/**
 * @brief Test that we are detecting invalid coordinates
 */
void coordFailTest(void) {
    size_t idx1 = 1000005;
    size_t calc[3] = {0,0,0};
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    CU_ASSERT(NULL != dim1);
    int ret1 = cartesianFromIndex(&idx1, calc, dim1);
    CU_ASSERT( ret1 == 1);
    free(dim1);

    size_t idx2 = 26777216;
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    CU_ASSERT(NULL != dim2);
    int ret2 = cartesianFromIndex(&idx2, calc, dim2);
    CU_ASSERT(ret2 == 1);
    free(dim2);

    size_t idx3 = 65537;
    size_t calc2[2] = {0,0};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    CU_ASSERT(NULL != dim3);
    int ret3 = cartesianFromIndex(&idx3, calc2, dim3);
    CU_ASSERT(ret3 == 1);
    free(dim3);
}

/**
 * @brief Test that we are getting valid index values from coordinates
 */
void indexTest(void) {

    size_t idx1 = 325505;
    size_t calc1 = 0;
    size_t coord1[3] = {5,55,32};
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    CU_ASSERT(NULL != dim1);
    int ret1 = indexFromCartesian(dim1, &calc1, coord1);
    CU_ASSERT( ret1 == 0);
    CU_ASSERT(calc1 == idx1);
    free(dim1);

    size_t idx2 = 2111237;
    size_t calc2 = 0;
    size_t coord2[3] = {5,55,32};
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    CU_ASSERT(NULL != dim2);
    int ret2 = indexFromCartesian(dim2, &calc2, coord2);
    CU_ASSERT(ret2 == 0);
    CU_ASSERT(calc2 == idx2);
    free(dim2);

    size_t idx3 = 56909;
    size_t calc3 = 0;
    size_t coord3[2] = {77,222};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    CU_ASSERT(NULL != dim3);
    int ret3 = indexFromCartesian(dim3, &calc3, coord3);
    CU_ASSERT(ret3 == 0);
    CU_ASSERT(calc3 == idx3);
    free(dim3);
}

/**
 * @brief Test that we are detecting invalid index values from coordinates.
 */
void indexFailTest(void) {
    size_t calc1 = 0;
    size_t bad1[3] = {10,42,101};
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    CU_ASSERT(NULL != dim1);
    int ret1 = indexFromCartesian(dim1, &calc1, bad1);
    CU_ASSERT( ret1 == 1);

    free(dim1);

    size_t calc2 = 0;
    size_t bad2[3] = {256,256,256};
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    CU_ASSERT(NULL != dim2);
    int ret2 = indexFromCartesian(dim2, &calc2, bad2);
    CU_ASSERT(ret2 == 1);
    free(dim2);

    size_t calc3 = 0;
    size_t bad3[2] = {10,258};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    CU_ASSERT(NULL != dim3);
    int ret3 = indexFromCartesian(dim3, &calc3, bad3);
    CU_ASSERT(ret3 == 1);
    free(dim3);
}

void badDataTests(void) {
    size_t idx1 = 1000005;
    size_t calcarr[3] = {0,0,0};
    size_t coord3[3] = {55,24,32};
    size_t calcidx = 0;
    struct dimensions_t *nulldim = NULL;
    size_t *nullidx = NULL;
    size_t *nullarr = NULL;
    struct dimensions_t *dim = createDimensions(3,100,100,100);

    CU_ASSERT(NULL != dim);
    //Bad inputs
    int ret1 = cartesianFromIndex(nullidx, calcarr, dim);
    CU_ASSERT( ret1 == -1);
    ret1 = 0;
    ret1 = cartesianFromIndex(&idx1, nullarr, dim);
    CU_ASSERT( ret1 == -1);
    ret1 = 0;
    ret1 = cartesianFromIndex(&idx1, calcarr, nulldim);
    CU_ASSERT( ret1 == -1);
    ret1 = 0;

    //Bad spatial inputs
    //NULL dimensions
    ret1 = indexFromCartesian(nulldim, &calcidx, coord3);
    CU_ASSERT(ret1 == -1);
    ret1 = 0;
    //NULL output ref
    ret1 = indexFromCartesian(dim, nullidx, coord3);
    CU_ASSERT(ret1 == -1);
    ret1 = 0;
    //Too few coords
    ret1 = indexFromCartesian(dim, &calcidx, nullarr);
    CU_ASSERT(ret1 == -1);
    ret1 = 0;

    free(dim);
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

    if (NULL == CU_add_test(pSuite, "Bad data", badDataTests)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
