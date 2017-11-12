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

#endif //GRAPHDATA_HASHES_H
