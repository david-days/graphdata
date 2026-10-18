//
// Created by david on 11/18/17.
//
#include <graphData.h>
#include <impl/hashgraph.h>
#include <util/hashes.h>
#include <util/crudops.h>
#include <stdlib.h>

/** Upper bound passed to maxEratosthenesPrime for the initial bucket count (yields 17). */
#define HASH_INIT_PRIME_BOUND 18u

/**
 * @brief Compute grow_at as ~3/4 of bucketcount.
 */
static size_t hashGrowThreshold(size_t bucketcount) {
    return (bucketcount * 3u) / 4u;
}

/**
 * @brief Allocate and zero a bucket array of the given length.
 */
static struct node_t **allocBuckets(size_t bucketcount) {
    struct node_t **buckets = (struct node_t **)calloc(bucketcount, sizeof(struct node_t *));
    return buckets;
}

/**
 * @brief Free every node (and its edges) in all buckets, then the bucket array.
 */
static void freeAllBuckets(struct node_t **buckets, size_t bucketcount) {
    if (buckets == NULL) {
        return;
    }
    for (size_t i = 0; i < bucketcount; i++) {
        struct node_t *curr = buckets[i];
        while (curr != NULL) {
            struct node_t *next = curr->next;
            if (curr->edges != NULL) {
                destroyEdges((void **)&curr->edges);
            }
            free(curr);
            curr = next;
        }
        buckets[i] = NULL;
    }
    free(buckets);
}

/**
 * @brief Initialize the graph using the hashtable adjacency list structure.
 *
 * @param g Graph structure to be operated upon.
 * @returns GRAPH_OK if successful; GRAPH_ERR of there was a problem.
 */
int hashGraphInit(struct graph_t *g) {
    if (g == NULL) {
        return GRAPH_ERR;
    }

    size_t bound = HASH_INIT_PRIME_BOUND;
    size_t bucketsz = maxEratosthenesPrime(&bound);
    if (bucketsz < 2u) {
        return GRAPH_ERR;
    }

    struct hashdata_t *meta = (struct hashdata_t *)malloc(sizeof(struct hashdata_t));
    if (meta == NULL) {
        return GRAPH_ERR;
    }
    meta->bucketcount = bucketsz;
    meta->nodecount = 0;
    meta->edgecount = 0;
    meta->grow_at = hashGrowThreshold(bucketsz);

    struct node_t **buckets = allocBuckets(bucketsz);
    if (buckets == NULL) {
        free(meta);
        return GRAPH_ERR;
    }

    g->metaImpl = meta;
    g->nodeImpl = buckets;
    g->edgeImpl = NULL;
    g->capImpl = NULL;
    g->flowImpl = NULL;
    return GRAPH_OK;
}

/**
 * @brief Clear out the underlying data structures for the given HASHED graph.
 *
 * @param g Graph to be cleared and memory deallocated
 * @return GRAPH_OK if successful; otherwise, GRAPH_ERR.
 */
int hashGraphFree(struct graph_t *g) {
    if (g == NULL || (g->gtype & HASHED) != HASHED) {
        return GRAPH_ERR;
    }

    struct hashdata_t *meta = (struct hashdata_t *)g->metaImpl;
    struct node_t **buckets = (struct node_t **)g->nodeImpl;
    size_t bucketcount = (meta != NULL) ? meta->bucketcount : 0;

    freeAllBuckets(buckets, bucketcount);
    g->nodeImpl = NULL;

    if (meta != NULL) {
        free(meta);
        g->metaImpl = NULL;
    }

    g->edgeImpl = NULL;
    g->capImpl = NULL;
    g->flowImpl = NULL;
    return GRAPH_OK;
}
