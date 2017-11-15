//
// Created by david on 11/14/17.
//

#include <CUnit/Basic.h>
#include <util/graphcomp.h>

void minNodeTest(void) {
    size_t big = 1000;
    size_t small = 5;
    size_t *result = NULL;
    result = minNode(&big, &small);
    CU_ASSERT(result == &small);
    result = NULL;
    result = minNode(&small, &big);
    CU_ASSERT(result == &small);
}

void maxNodeTest(void) {
    size_t big = 1000;
    size_t small = 5;
    size_t *result = NULL;
    result = maxNode(&big, &small);
    CU_ASSERT(result == &big);
    result = NULL;
    result = maxNode(&small, &big);
    CU_ASSERT(result == &big);
}

void minValTest(void) {
    double big = 3200.0;
    double small = 12.0;
    double *result = NULL;
    result = minVal(&big, &small);
    CU_ASSERT(result == &small);
    result = NULL;
    result = minVal(&small, &big);
    CU_ASSERT(result == &small);
}

void maxValTest(void) {
    double big = 3200.0;
    double small = 12.0;
    double *result = NULL;
    result = maxVal(&big, &small);
    CU_ASSERT(result == &big);
    result = NULL;
    result = maxVal(&small, &big);
    CU_ASSERT(result == &big);
}

int main (int argc, char** argv) {
    CU_pSuite  pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("MinMax Suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Min size_t", minNodeTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Max size_t", maxNodeTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Min double", minValTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Max double", maxValTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}