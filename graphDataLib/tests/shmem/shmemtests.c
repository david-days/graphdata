//
// Created by david on 8/21/25.
//

#include <check.h>
#include <graphInit.h>
#include <graphOps.h>
#include <util/cartesian.h>

Suite * init_suite(void) {
    Suite * s;
    TCase *tc_core;

    s = suite_create("ShMem");

    tc_core = tcase_create("Core");
    // add tests by name here
    // tcase_add_test(tc_core, test_name);

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
