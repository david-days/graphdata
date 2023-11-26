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
 * @param typeFlags Type of graph implementation to be created. Flag values set underlying structures and metadata.
 * @param shareFlags Access control flags (privated, shared, file-based, etc)
 * @param lblCount Number of label nodes to be used within the graph.  Required for LABELED flag; ignored for all others.
 * @param dims Dimensional parameters structure.  Required for ARRAY graphs; otherwise, may be NULL.  The returned graph will
 * hold the reference to the structure that was passed.
 * @return If successful and valid, initialized graph structure, according to the flags.  Otherwise, a NULL pointer.
 */
struct graph_t * initGraph(enum GRAPHDOMAIN typeFlags, size_t lblCount, struct dimensions_t *dims);

/**
 * @brief Stand up a shared graph according to the settings. This may be as a memory-only or file-backed structure.
 * 
 * @param gtype_flags Type of graph to create
 * @param gacc_flags Sharing/access settings
 * @param lblCount Number of labels to be used
 * @param dims Dimensional characteristics
 * @param sharedMeta Shared metaadata structure
 * @return reference to a fully initialized graph structure
 */
struct graph_t * initSharedGraph(enum GRAPHDOMAIN *gtype_flags, enum GRAPHACCESS *gacc_flags, const size_t lblCount, struct dimensions_t *dims, void *sharedMeta);

/**
 * @brief Create and fill the graphOps_t structure that handles basic operations for the graph
 *
 * Using the graph_t implementation pass, this function creates a graphops_t structure necessary to handle basic functions
 * for the given graph.
 *
 * @param graph Graph structure being used
 * @return Pointer to a new graphops_t structure, if successful; otherwise, a pointer to NULL
 *
 */
struct graphops_t * getOperations(struct graph_t *graph);

/**
 * @brief Clear out the graph's underlying structures, and null out the memory
 *
 * All underlying graph structures will be cleared and the associated memory to the structures freed.
 *
 * @param graph Graph to be cleared
 * @return 1 if successful; otherwise, 0.
 */
short clearGraph(struct graph_t *graph);


#endif //GRAPHDATA_GRAPHINIT_H
