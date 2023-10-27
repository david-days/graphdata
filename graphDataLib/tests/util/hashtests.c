/**
 * Perform testing of functions in the util/hashes.c file
 *
 */

//
// Created by david on 11/14/17.
//

#include <check.h>
#include <stdlib.h>
#include <string.h>
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

/**
 * Using a know starting prime number, verify that the next prime number is larger
 * @param _i nextPrimeTest
 */
START_TEST(nextPrimeTest) {
    size_t expPrime = 43;
    size_t checkIdx = 45;
    size_t foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
    size_t next_Prime = nextPrime(foundPrime);
    ck_assert(next_Prime > foundPrime);
    
    checkIdx = 50;
    expPrime = 47;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
    next_Prime = nextPrime(foundPrime);
    ck_assert(next_Prime > foundPrime);

    checkIdx = 60;
    expPrime = 59;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
    next_Prime = nextPrime(foundPrime);
    ck_assert(next_Prime > foundPrime);

    checkIdx = 102;
    expPrime = 101;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
    next_Prime = nextPrime(foundPrime);
    ck_assert(next_Prime > foundPrime);

    checkIdx = 795;
    expPrime = 787;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
    next_Prime = nextPrime(foundPrime);
    ck_assert(next_Prime > foundPrime);

    checkIdx = 275;
    expPrime = 271;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
    next_Prime = nextPrime(foundPrime);
    ck_assert(next_Prime > foundPrime);


    checkIdx = 580;
    expPrime = 577;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
    next_Prime = nextPrime(foundPrime);
    ck_assert(next_Prime > foundPrime);

    checkIdx = 700;
    expPrime = 691;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    ck_assert(foundPrime == expPrime);
    next_Prime = nextPrime(foundPrime);
    ck_assert(next_Prime > foundPrime);
}
END_TEST


START_TEST(charHashTest) {
    char *char1 = "0123456789";
    char *char2 = "0123456789";
    char *revChar = "9876543210";
    
    size_t hash1 = superFastHash(char1, 10);
    size_t hash2 = superFastHash(char2, 10);
    size_t revHash = superFastHash(revChar, 10);
    
    ck_assert(hash1 == hash2);
    ck_assert(hash1 != revHash);
    ck_assert(hash2 != revHash);
}
END_TEST

Suite * init_suite(void) {
    Suite * s;
    TCase *tc_core;

    s = suite_create("Hash");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, primeNumberTest);
    tcase_add_test(tc_core, nextPrimeTest);
    tcase_add_test(tc_core, charHashTest);
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
