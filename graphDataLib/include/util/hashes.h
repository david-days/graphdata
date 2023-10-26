//
// Created by david on 11/12/17.
//

#ifndef GRAPHDATA_HASHES_H
#define GRAPHDATA_HASHES_H

#include <stdio.h>

/**
 * Calculate a unique hash for the char data.
 * SuperFastHash used from http://www.azillionmonkeys.com/qed/hash.html under LGPL 2.1 license
 * @param data character data (string value) to be hashed
 * @param len Length of the character data
 * @return hash value of the character data, if not null; otherwise, zero.
 */
size_t SuperFastHash (const char * data, int len);

/**
 * @brief Using the Sieve of Eratosthenes, calculate the max prime value below the given value
 *
 * Quick and dirty (not necessarily computationally quick) way to get the max prime number below the given
 * value.
 *
 * @param idx Size value to search the max prime number below
 * @return Max prime number within the range given by the input, if successful; otherwise 0
 */
size_t maxEratosthenesPrime(size_t *idx);

/**
 * @brief Utility function to determine next-ish higher prime number
 * 
 * This method is not necessarily mathematically rigorous in determining the "next"
 * prime number, but rather is used to obtain a prime number that is in the range of
 * \f$2^n * currPrime\f$
 * 
 * The intent is to create a modulo for the hashtable to prevent collisions
 * @param currPrime 
 * @return a higher prime values on the order of \f$2^n * currPrime\f$ for some n
 */
size_t nextPrime(size_t currPrime);

#endif //GRAPHDATA_HASHES_H
