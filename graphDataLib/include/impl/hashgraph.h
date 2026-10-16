//
// Created by david on 11/18/17.
//

#ifndef GRAPHDATA_HASHGRAPH_H
#define GRAPHDATA_HASHGRAPH_H

#include <graphData.h>

/**
 * @brief Metadata for HASHED (chained hashtable) graphs.
 *
 * nodeImpl is an array of bucket heads (struct node_t **), length bucketcount.
 * Chains use node_t.next within a bucket. Edges hang off node_t.edges.
 */
struct hashdata_t {
    /**
     * @brief Number of buckets (prime).
     */
    size_t bucketcount;
    /**
     * @brief Number of nodes currently stored.
     */
    size_t nodecount;
    /**
     * @brief Number of directed edge records stored.
     *
     * Undirected graphs store both directions, so each logical link counts as 2.
     */
    size_t edgecount;
    /**
     * @brief Rehash when nodecount would exceed this (~ 3/4 * bucketcount).
     */
    size_t grow_at;
};

/**
 * @brief Initialize the graph using the hashtable adjacency list structure.
 *
 * Allocates a small prime-sized bucket array (start-small, grow-on-load).
 *
 * @param g Graph structure to be operated upon.
 * @returns EXIT_SUCCESS if successful; EXIT_FAILURE if there was a problem.
 */
int hashGraphInit(struct graph_t *g);

/**
 * @brief Clear out the underlying data structures for the given HASHED graph.
 *
 * @param g Graph to be cleared and memory deallocated
 * @return EXIT_SUCCESS if successful; otherwise, EXIT_FAILURE.
 */
int hashGraphFree(struct graph_t *g);


#endif //GRAPHDATA_HASHGRAPH_H
