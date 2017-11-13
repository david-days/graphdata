//
// Created by david on 11/12/17.
//

#ifndef GRAPHDATA_GRAPHCOMP_H
#define GRAPHDATA_GRAPHCOMP_H

#include <stdio.h>

/**
 * Compare to nodeid values, returning the minimum value (if different)
 * @param a first value to be compared
 * @param b second value to be compared
 * @return pointer to first value, if same or smaller; otherwise, pointer to second value.
 */
size_t * minNode(size_t *a, size_t *b);

/**
 * Compare nodeid values, returning the maximum value (if different)
 * @param a first value to be compared
 * @param b second value to be compared
 * @return pointer to the first value, if same or larger, otherwise, pointer to second value
 */
size_t * maxNode(size_t *a, size_t *b);

/**
 * Compare double values, returning the minimum value, if different.
 * @param a First value to be compared
 * @param b Second value to be compared
 * @return pointer to the first value, if same or smaller; otherwise, pointer to the second value
 */
double * minVal(double *a, double *b);

/**
 * Compare double values, returning the maximum value, if different.
 * @param a First value to be compared
 * @param b Second value to be compared
 * @return pointer to the first value, if same or larger; otherwise, pointer to the second value
 */
double * maxVal(double *a, double *b);

#endif //GRAPHDATA_GRAPHCOMP_H
