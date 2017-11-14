/**
 * Header for comparative and extraction functions (max, min, largest, smallest, etc).
 * definitions and function here should relate directly to commonly-used utility functions, and try to avoid
 * implementation-specific functions.
 */

#ifndef GRAPHDATA_GRAPHCOMP_H
#define GRAPHDATA_GRAPHCOMP_H

#include <stdio.h>
#include <graphdata.h>

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

/**
 * Given an edge path, return the min capacity along that path.
 * @param path linked-list of edges, representing a path
 * @return minimum value along the given path, if path exists; otherwise, value < 0 if *path is NULL
 */
double minCapacity(const edge *path);

/**
 * Given an edge path, return the max capacity along that path.
 * @param path  linked-listof edges, representing a path
 * @return maximum value along the given path, if path exists; otherwise, value < 0 if *path is NULL
 */
double maxCapacity(const edge *path);

#endif //GRAPHDATA_GRAPHCOMP_H
