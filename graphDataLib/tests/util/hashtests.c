/**
 * Perform testing of functions in the util/hashes.c file
 *
 */

//
// Created by david on 11/14/17.
//

#include <check.h>
#include <stdlib.h>
#include <util/hashes.h>

/**
 * @brief Using known primes, verify that the prime number calculations are correct.
 */
START_TEST(primeNumberTest) {
    size_t expPrime = 43;
    size_t checkIdx = 45;
    size_t foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);

    checkIdx = 50;
    expPrime = 47;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);

    checkIdx = 60;
    expPrime = 59;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);

    checkIdx = 102;
    expPrime = 101;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);

    checkIdx = 795;
    expPrime = 787;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);

    checkIdx = 275;
    expPrime = 271;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);


    checkIdx = 580;
    expPrime = 577;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);

    checkIdx = 700;
    expPrime = 691;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
}
END_TEST

Suite * init_suite(void) {
    Suite * s;
    TCase *tc_core;

    s = suite_create("Hash");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, primeNumberTest);
    suite_add_tcase(s, tc_core);

    return s;
}



int main(void) {
    int number_failed;
    Suite * s;
    SRunner *sr;

    s = init_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
