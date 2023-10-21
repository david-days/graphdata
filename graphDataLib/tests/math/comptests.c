//
// Created by david on 11/14/17.
//

#include <check.h>
#include <stdlib.h>
#include <util/graphcomp.h>

START_TEST(minNodeTest) {
    size_t big = 1000;
    size_t small = 5;
    size_t *result = NULL;
    result = minNode(&big, &small);
    ck_assert(result == &small);
    result = NULL;
    result = minNode(&small, &big);
    ck_assert(result == &small);
}
END_TEST

START_TEST(maxNodeTest) {
    size_t big = 1000;
    size_t small = 5;
    size_t *result = NULL;
    result = maxNode(&big, &small);
    ck_assert(result == &big);
    result = NULL;
    result = maxNode(&small, &big);
    ck_assert(result == &big);
}
END_TEST

START_TEST(minValTest) {
    double big = 3200.0;
    double small = 12.0;
    double *result = NULL;
    result = minVal(&big, &small);
    ck_assert(result == &small);
    result = NULL;
    result = minVal(&small, &big);
    ck_assert(result == &small);
}
END_TEST

START_TEST(maxValTest) {
    double big = 3200.0;
    double small = 12.0;
    double *result = NULL;
    result = maxVal(&big, &small);
    ck_assert(result == &big);
    result = NULL;
    result = maxVal(&small, &big);
    ck_assert(result == &big);
}
END_TEST

Suite * init_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Compare");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, minNodeTest);
    tcase_add_test(tc_core, maxNodeTest);
    tcase_add_test(tc_core, minValTest);
    tcase_add_test(tc_core, maxValTest);

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

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}