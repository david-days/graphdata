//
// Created by david on 11/18/17.
//

#include <check.h>
#include <graphData.h>
#include <string.h>
#include <util/cartesian.h>
#include <util/crudops.h>


static void checkArrays(size_t *expected, size_t *calced, size_t len) {
    ck_assert(expected != NULL);
    ck_assert(calced != NULL);
    for (int i=0;i<len;i++) {
        ck_assert(expected[i] == calced[i]);
    }
}

/**
 * @brief Test that we are getting correct index lengths
 */
START_TEST(indexLengthTest) {
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    ck_assert(NULL != dim1);
    size_t len1 = cartesianIndexLength(dim1);
    ck_assert(len1 == 1000000);
    free(dim1);

    struct dimensions_t *dim2 = createDimensions(2,10,10);
    ck_assert(NULL != dim2);
    size_t len2 = cartesianIndexLength(dim2);
    ck_assert(len2 == 100);
    free(dim2);

    struct dimensions_t *dim3 = createDimensions(2, 256,256);
    ck_assert(NULL != dim3);
    size_t len3 = cartesianIndexLength(dim3);
    ck_assert(len3 == 65536);
    free(dim3);
}
END_TEST

/**
 * @brief Test that we are getting valid coordinates
 */
START_TEST(coordTest) {

    size_t coord3d[3] = { 5, 55, 32 };
    size_t coord2d[2] = {77, 222};
    size_t idx1 = 325505;
    size_t calc1[3] = {0,0,0};
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    ck_assert(NULL != dim1);
    int ret1 = cartesianFromIndex(&idx1, calc1, dim1);
    ck_assert( ret1 == 0);
    printf("Checking arrays for coordTest dim1");
    checkArrays(coord3d, calc1, dim1->dimcount);
    printf("dim1 complete");
    free(dim1);

    size_t idx2 = 2111237;
    size_t calc2[3] = {0,0,0};
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    ck_assert(NULL != dim2);
    int ret2 = cartesianFromIndex(&idx2, calc2, dim2);
    ck_assert(ret2 == 0);
    printf("Checking arrays for coordTest dim2");
    checkArrays(coord3d, calc2, dim2->dimcount);
    printf("dim2 complete");
    free(dim2);

    size_t idx3 = 56909;
    size_t calc3[2] = {0,0};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    ck_assert(NULL != dim3);
    int ret3 = cartesianFromIndex(&idx3, calc3, dim3);
    ck_assert(ret3 == 0);
    printf("Checking arrays for coordTest dim3");
    checkArrays(coord2d, calc3, dim3->dimcount);
    printf("dim3 complete");
    free(dim3);
}
END_TEST

/**
 * @brief Test that we are detecting invalid coordinates
 */
START_TEST(coordFailTest) {
    size_t idx1 = 1000005;
    size_t calc[3] = {0,0,0};
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    ck_assert(NULL != dim1);
    int ret1 = cartesianFromIndex(&idx1, calc, dim1);
    ck_assert( ret1 == 1);
    free(dim1);

    size_t idx2 = 26777216;
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    ck_assert(NULL != dim2);
    int ret2 = cartesianFromIndex(&idx2, calc, dim2);
    ck_assert(ret2 == 1);
    free(dim2);

    size_t idx3 = 65537;
    size_t calc2[2] = {0,0};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    ck_assert(NULL != dim3);
    int ret3 = cartesianFromIndex(&idx3, calc2, dim3);
    ck_assert(ret3 == 1);
    free(dim3);
}
END_TEST

/**
 * @brief Test that we are getting valid index values from coordinates
 */
START_TEST(indexTest) {

    size_t idx1 = 325505;
    size_t calc1 = 0;
    size_t coord1[3] = {5,55,32};
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    ck_assert(NULL != dim1);
    int ret1 = indexFromCartesian(dim1, &calc1, coord1);
    ck_assert( ret1 == 0);
    ck_assert(calc1 == idx1);
    free(dim1);

    size_t idx2 = 2111237;
    size_t calc2 = 0;
    size_t coord2[3] = {5,55,32};
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    ck_assert(NULL != dim2);
    int ret2 = indexFromCartesian(dim2, &calc2, coord2);
    ck_assert(ret2 == 0);
    ck_assert(calc2 == idx2);
    free(dim2);

    size_t idx3 = 56909;
    size_t calc3 = 0;
    size_t coord3[2] = {77,222};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    ck_assert(NULL != dim3);
    int ret3 = indexFromCartesian(dim3, &calc3, coord3);
    ck_assert(ret3 == 0);
    ck_assert(calc3 == idx3);
    free(dim3);
}
END_TEST

/**
 * @brief Test that we are detecting invalid index values from coordinates.
 */
START_TEST(indexFailTest) {
    size_t calc1 = 0;
    size_t bad1[3] = {10,42,101};
    struct dimensions_t *dim1 = createDimensions(3,100,100,100);
    ck_assert(NULL != dim1);
    int ret1 = indexFromCartesian(dim1, &calc1, bad1);
    ck_assert( ret1 == 1);

    free(dim1);

    size_t calc2 = 0;
    size_t bad2[3] = {256,256,256};
    struct dimensions_t *dim2 = createDimensions(3,256,256,256);
    ck_assert(NULL != dim2);
    int ret2 = indexFromCartesian(dim2, &calc2, bad2);
    ck_assert(ret2 == 1);
    free(dim2);

    size_t calc3 = 0;
    size_t bad3[2] = {10,258};
    struct dimensions_t *dim3 = createDimensions(2,256,256);
    ck_assert(NULL != dim3);
    int ret3 = indexFromCartesian(dim3, &calc3, bad3);
    ck_assert(ret3 == 1);
    free(dim3);
}
END_TEST

START_TEST(badDataTests) {
    size_t idx1 = 1000005;
    size_t calcarr[3] = {0,0,0};
    size_t coord3[3] = {55,24,32};
    size_t calcidx = 0;
    struct dimensions_t *nulldim = NULL;
    size_t *nullidx = NULL;
    size_t *nullarr = NULL;
    struct dimensions_t *dim = createDimensions(3,100,100,100);

    ck_assert(NULL != dim);
    //Bad inputs
    int ret1 = cartesianFromIndex(nullidx, calcarr, dim);
    ck_assert( ret1 == -1);
    ret1 = 0;
    ret1 = cartesianFromIndex(&idx1, nullarr, dim);
    ck_assert( ret1 == -1);
    ret1 = 0;
    ret1 = cartesianFromIndex(&idx1, calcarr, nulldim);
    ck_assert( ret1 == -1);
    ret1 = 0;

    //Bad spatial inputs
    //NULL dimensions
    ret1 = indexFromCartesian(nulldim, &calcidx, coord3);
    ck_assert(ret1 == -1);
    ret1 = 0;
    //NULL output ref
    ret1 = indexFromCartesian(dim, nullidx, coord3);
    ck_assert(ret1 == -1);
    ret1 = 0;
    //Too few coords
    ret1 = indexFromCartesian(dim, &calcidx, nullarr);
    ck_assert(ret1 == -1);
    ret1 = 0;

    free(dim);
}
END_TEST

Suite * init_suite(void) {
    Suite * s;
    TCase *tc_core;

    s = suite_create("Spatial");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, indexLengthTest);
    tcase_add_test(tc_core, coordTest);
    tcase_add_test(tc_core, coordFailTest);
    tcase_add_test(tc_core, indexTest);
    tcase_add_test(tc_core, indexFailTest);
    tcase_add_test(tc_core, badDataTests);
    suite_add_tcase(s, tc_core);

    return s;
}



int main(void) {
    int number_failed;
    Suite * s;
    SRunner *sr;

    s = init_suite();
    sr = srunner_create(s);
    srunner_set_log(sr, "spatialTestLog.txt");
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
