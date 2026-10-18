//
// Created by david on 11/11/17.
//

#ifndef GRAPHDATA_GRAPHINIT_H
#define GRAPHDATA_GRAPHINIT_H

#include <graphData.h>
#include "graphOps.h"


/**
 * @brief Initialize a graph according to the flags set in the GRAPHDOMAIN value.
 *
 * Using the flags set in the typeflags parameter, create a graph structure capable of supporting the requirements in an
 * efficient (memory or Big-O) manner.
 *
 * The parameter gtype holds the flags for what graph is desired.  For example, For a directed graph, used to represent a set of cartesian
 * coordinates and no extra label nodes, pass in the value created by DIRECTED | LINKED | SPATIAL | UNLABELED.
 *
 * If an empty set of flags are passed to the typeflags parameter, the returned graph will be UNDIRECTED | LINKED | GENERIC | UNLABELED.
 *
 * @param gtype Type of graph implementation to be created. Flag values set underlying structures and metadata.
 * @param lblcount Number of label nodes to be used within the graph.  Required for LABELED flag; ignored for all others.
 * @param dims Dimensional parameters structure.  Required for ARRAY graphs; otherwise, may be NULL.  The returned graph will
 * hold the reference to the structure that was passed.
 * @return If successful and valid, initialized graph structure, according to the flags.  Otherwise, a NULL pointer.
 */
struct graph_t * initGraph(enum GRAPHDOMAIN typeflags, size_t lblcount, struct dimensions_t *dims);

/**
 * @brief PROVISIONAL: Attach a read-only accessor to an existing shared graph.
 *
 * Supported for SHARED_MEM and SHARED_MMAP sources only. Creates a second graph_t
 * that maps the same backing store as @p src with RD_ONLY semantics so write ops
 * fail via graphops policy (and OS read-only attach where applicable).
 *
 * This is intentionally not the long-term multi-process reader API (no key/path
 * publication, handshake, or versioning). That work is deferred.
 *
 * The returned graph shares @p src->dims (do not destroy dims until both graphs
 * are cleared). Free the readonly graph before the writable owner.
 *
 * @param src existing writable SHARED_MEM or SHARED_MMAP graph
 * @return new RD_ONLY graph on success; NULL on failure
 */
struct graph_t * attachReadonlyGraph(struct graph_t *src);

/**
 * @brief Create and fill the graphOps_t structure that handles basic operations for the graph
 *
 * Using the graph_t implementation pass, this function creates a graphops_t structure necessary to handle basic functions
 * for the given graph.
 *
 * @param g Graph structure being used
 * @return Pointer to a new graphops_t structure, if successful; otherwise, a pointer to NULL
 *
 */
struct graphops_t * getOperations(struct graph_t *g);

/**
 * @brief Clear out the graph's underlying structures, and null out the memory
 *
 * All underlying graph structures will be cleared and the associated memory to the structures freed.
 *
 * @param g Graph to be cleared
 * @return EXIT_SUCCESS if successful; otherwise, EXIT_FAILURE.
 */
int clearGraph(struct graph_t *g);


#endif //GRAPHDATA_GRAPHINIT_H
