/**
 * Perform testing of functions in the util/hashes.c file
 *
 */

//
// Created by david on 11/14/17.
//

#include <CUnit/Basic.h>
#include <util/hashes.h>

/**
 * @brief Using known primes, verify that the prime number calculations are correct.
 */
void primeNumberTest(void) {
    size_t expPrime = 43;
    size_t checkIdx = 45;
    size_t foundPrime = maxEratosthenesPrime(&checkIdx);
    CU_ASSERT(foundPrime == expPrime);

    checkIdx = 50;
    expPrime = 47;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    CU_ASSERT(foundPrime == expPrime);

    checkIdx = 60;
    expPrime = 59;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    CU_ASSERT(foundPrime == expPrime);

    checkIdx = 102;
    expPrime = 101;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    CU_ASSERT(foundPrime == expPrime);

    checkIdx = 795;
    expPrime = 787;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    CU_ASSERT(foundPrime == expPrime);

    checkIdx = 275;
    expPrime = 271;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    CU_ASSERT(foundPrime == expPrime);


    checkIdx = 580;
    expPrime = 577;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    CU_ASSERT(foundPrime == expPrime);

    checkIdx = 700;
    expPrime = 691;
    foundPrime = maxEratosthenesPrime(&checkIdx);
    CU_ASSERT(foundPrime == expPrime);
}




/**
 * @brief Run the tests for hash calculations
 *
 * @param argc
 * @param argv
 * @return
 */
int main (int argc, char** argv) {
    CU_pSuite  pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("Hash Utilities Suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "Prime Numbers", primeNumberTest)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}