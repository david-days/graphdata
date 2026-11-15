//
// Created by david on 11/12/17.
//
#include <util/graphcomp.h>

/**
 * Compare to nodeid values, returning the minimum value (if different)
 * @param a first value to be compared
 * @param b second value to be compared
 * @return pointer to first value, if same or smaller; otherwise, pointer to second value.
 */
size_t * minNode(size_t *a, size_t *b) {
    return *a <= *b ? a:b;
}

/**
 * Compare nodeid values, returning the maximum value (if different)
 * @param a first value to be compared
 * @param b second value to be compared
 * @return pointer to the first value, if same or larger, otherwise, pointer to second value
 */
size_t * maxNode(size_t *a, size_t *b) {
    return *a >= *b ? a:b;
}

/**
 * Compare double values, returning the minimum value, if different.
 * @param a First value to be compared
 * @param b Second value to be compared
 * @return pointer to the first value, if same or smaller; otherwise, pointer to the second value
 */
double * minVal(double *a, double *b) {
    return *a <= *b ? a:b;
}

/**
 * Compare double values, returning the maximum value, if different.
 * @param a First value to be compared
 * @param b Second value to be compared
 * @return pointer to the first value, if same or larger; otherwise, pointer to the second value
 */
double * maxVal(double *a, double *b) {
    return *a >= *b ? a:b;
}

/**
 * Given an edge path, return the min capacity along that path.
 * @param path linked-list of edges, representing a path
 * @return minimum value along the given path, if path exists; otherwise, value < 0 if *path is NULL
 */
double minCapacity(const struct edge_t *path) {
    double minCap = -1.0;
    if (path != NULL) {
        struct edge_t *curr, *next;
        curr = (struct edge_t *)path;
        next = (struct edge_t *)curr->next;
        minCap = curr->cap;
        while (next != NULL) {
            minCap = *minVal(&minCap, &(next->cap));
            curr = next;
            next = (struct edge_t *)curr->next;
        }
    }
    return minCap;
}

/**
 * Given an edge path, return the max capacity along that path.
 * @param path  linked-list of edges, representing a path
 * @return maximum value along the given path, if path exists; otherwise, value < 0 if *path is NULL
 */
double maxCapacity(const struct edge_t *path) {
    double maxCap = -1.0;
    if (path != NULL) {
        struct edge_t *curr = NULL;
        struct edge_t *next = NULL;
        curr = (struct edge_t *)path;
        next = (struct edge_t *)curr->next;
        maxCap = curr->cap;
        while (next != NULL) {
            maxCap = *maxVal(&maxCap, &(next->cap));
            curr = next;
            next = (struct edge_t *)curr->next;
        }
    }
    return maxCap;
}