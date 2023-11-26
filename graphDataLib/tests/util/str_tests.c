//
// Created by david on 11/24/23.
//

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <util/strfuncs.h>
#include <uuid/uuid.h>

START_TEST(uuidGenTest) {

    for (int i=0;i<100;i++) {
        char *genid = (char *)malloc(UUID_STR_LEN);
        short didGen = gen_uuid(genid);
        ck_assert(didGen == 0);
        int uuidLen = strlen(genid);
        //length should be one shorter (not including the null termination
        ck_assert(uuidLen == UUID_STR_LEN-1);
    }    

}
END_TEST

Suite * init_suite(void) {
    Suite * s;
    TCase *tc_core;

    s = suite_create("Strings");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, uuidGenTest);
    suite_add_tcase(s, tc_core);

    return s;
}



int main(void) {
    Suite * s;
    SRunner *sr;

    s = init_suite();
    sr = srunner_create(s);

    srunner_set_log(sr, "strfuncsTestLog.txt");
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);

    const int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
