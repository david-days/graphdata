//
// Created by david on 11/12/17.
//

/**
 * Source code for SuperFastHash used from http://www.azillionmonkeys.com/qed/hash.html under LGPL 2.1 license
 */

#include <util/hashes.h>
#include <stdint.h>
#include <stdlib.h>


#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((size_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(size_t)(((const uint8_t *)(d))[0]) )
#endif

size_t SuperFastHash (const char * data, int len) {
    size_t hash = len, tmp;
    int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
            hash ^= hash << 16;
            hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
            hash += hash >> 11;
            break;
        case 2: hash += get16bits (data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1: hash += (signed char)*data;
            hash ^= hash << 10;
            hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

/**
 * @brief Using the Sieve of Eratosthenes, calculate the max prime value below the given value
 *
 * Quick and dirty (not necessarily computationally quick) way to get the max prime number below the given
 * value.
 *
 * Implementation based off example code at https://www.programminglogic.com/the-sieve-of-eratosthenes-implemented-in-c/
 *
 * @param idx Size value to search the max prime number below
 * @return Prime number within the range given by the input, if successful; otherwise, returns 0
 */
size_t maxEratosthenesPrime(size_t *idx) {
    size_t mprime = 0;
    size_t plen = *idx;
    char *sieve = (char *)malloc(plen * sizeof(size_t));
    if (sieve != NULL) {
        //mark as possible
        for (size_t i=2;i<plen;i++) *(sieve + i) = 1;

        for (size_t i=2;i<plen;i++) {
            if (sieve[i]) {
                for (size_t j = i;i*j<plen;j++) {
                    sieve[i*j] = 0;
                }
            }
        }

        //scan for max
        size_t ppos = 0;
        for (size_t i=0;i<plen;i++) {
            if (sieve[i] > 0) ppos = i;
        }
        mprime = ppos;
    }
    free(sieve);
    return mprime;
}