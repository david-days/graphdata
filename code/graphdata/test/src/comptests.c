//
// Created by david on 11/14/17.
//

#include <CUnit/Basic.h>
#include <util/graphcomp.h>

void simpleTest(void) {
    CU_ASSERT(1 == 1);
}

int main (int argc, char** argv) {
    CU_pSuite  pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Test Suite 1", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Simple Test Example", simpleTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}