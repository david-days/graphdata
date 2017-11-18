//
// Created by david on 11/12/17.
//

#ifndef GRAPHDATA_GRAPHOPS_H
#define GRAPHDATA_GRAPHOPS_H

#include <graphdata.h>

//Read functions to extract data
/**
 * @brief Function pointer definition for getting the node count;
 * @param g Graph structure in question
 * @return Count of nodes, if graph is not null; otherwise, return 0
 */
typedef size_t (*funcNodeCount)(graph *g);
/**
 * @brief Function pointer to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
typedef size_t (*funcEdgeCount)(graph *g);
/**
 * @brief Function pointer to retrieve a node structure reference.
 *
 * Implementation-specific on whether this
 * structure is part of the original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, pointer to NULL
 */
typedef node * (*funcGetNode)(const size_t *nodeid, const graph *g);
/**
 * @brief Function pointer to retrieve a edge structure reference.
 *
 * Implementation-specific on whether this structure is part of the
 * original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param u nodeid of the starting edge.
 * @param v nodeid of the ending edge.
 * @param g Graph structure in question
 * @return pointer to the edge structure, if found; otherwise, pointer to NULL.
 */
typedef edge * (*funcGetEdge)(const size_t *u, const size_t *v, const graph *g);
/**
 * @brief Function pointer to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of node references, starting with the given node, if found; otherwise, pointer to NULL.
 */
typedef node * (*funcGetNeighbors)(const size_t *nodeid, const graph *g);
/**
 * @brief Function pointer to retrieve linked-list of edges from a given node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of edges starting from the given node, if found; otherwise, pointer to NULL.
 */
typedef edge * (*funcGetEdges)(const size_t *nodeid, const graph *g);
/**
 * @brief Function pointer to retrieve linked-list path of nodes from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of nodes, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
typedef node * (*funcGetNodePath)(const size_t *uid, const size_t *vid, const graph *g);
/**
 * @brief Function pointer to retrieve linked-list of path edges from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of edges, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
typedef edge * (*funcGetEdgePath)(const size_t *uid, const size_t *vid, const graph *g);

//Write functions to modify graph
/**
 * @brief Function pointer to add a node to a given graph.
 * Not all implementations may use this (for example, fixed-size graphs such as spatial ARRAY implementations).
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return -1 if there was an error, 0 if the node was successfully added, 1 if the node already exists.
 */
typedef int (*funcAddNode)(const size_t *nodeid, graph *g);
/**
 * @brief Function pointer to add an edge to a given graph.
 * Not all implementations may use this (for example, fixed-size ARRAY implementations representing a set domain of nodes and relationships).
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return -1 if there was an error; 0 if the edge was successfully added; 1 if the edge already exists.
 */
typedef int (*funcAddEdge)(const size_t *uid, const size_t *vid, double *cap, graph *g);
/**
 * @brief Function pointer to set the capacity (cost, weight, etc.) of an edge in the given graph.
 * @param uid identifier of the edge start
 * @param vid identifier of the edge ending.
 * @param cap capacity value to be set
 * @param g Graph structure in question
 * @return -1 if there was an error; 0 if the capacity was successfully set; 1 if the value was unchanged
 */
typedef int (*funcSetCapacity)(const size_t *uid, const size_t *vid, const double *cap, graph *g);

/**
 * @brief Function pointer to "reset" the graph according to the given argument pointer.
 *
 * For some implementations, it is more efficient to reuse the existing graph structure and perform a "zero-out"
 * of the data, rather than rebuilding from scratch.  This function pointer provides that option.
 * @param g Graph structure to be zeroed or modified according to reset logic
 * @param args Arguments to be used in the reset process, if necessary
 * @param callback Callback to be executed when graph has been reset.
 * @return -1 if there was an error during the reset; 0 if the reset completed; 1 if there was a change the requires re-evaluation
 */
typedef int (*funcResetGraph)(graph *g, void *args, void (*callback)(void));



/**
 * @brief Structure relating graph data to implementations
 *
 * This is the structure containing both a reference to the graph data and the supporting basic operations.
 * Function pointers (according to the typedefs) are set if there is a function implementation associated with the type
 * of graph and implementation.
 */
struct graphops_t {
    //Graph structure
    /**
     * @brief Graph data structure
     */
    graph *g;

    //Getters
    /**
     * @brief Return count of nodes
     */
    funcNodeCount nodeCount;
    /**
     * @brief Return count of edges
     */
    funcEdgeCount edgeCount;
    /**
     * @brief Return a given node (by identifier)
     */
    funcGetNode getNode;
    /**
     * @brief Return a given edge (by start-end identifiers)
     */
    funcGetEdge getEdge;
    /**
     * @brief Return a linked-list of neighbors
     */
    funcGetNeighbors getNeighbors;
    /**
     * @brief Return a linked-list of edges starting at this node
     */
    funcGetEdges getEdges;

    //Modifiers
    /**
     * @brief Add a node to the graph.
     * Expected to be NULL (no implementation) for fixed-size graph structures.
     */
    funcAddNode addNode;
    /**
     * @brief Add an edge to the graph.
     * Expected to be NULL (no implementation) for fixed-size graph structures.
     */
    funcAddEdge addEdge;
    /**
     * Set the capacity for a given edge.
     */
    funcSetCapacity setCapacity;

    //Traverse functions
    /**
     * @brief Return a list of node (hops) from the start to the end.
     * Means of finding the path is implementation-specific
     */
    funcGetNodePath nodePath;
    /**
     * @brief Return a list of edges traversing from the start to finish.
     * Means of finding the path is implementation-specific.
     */
    funcGetEdgePath edgePath;
    /**
     * @brief Reset the graph to an initial state, according to implementation logic
     * Means of resetting the graph to an initial state without having to re-implement cleanup logic
     * or creating a new graph structure.
     */
    funcResetGraph resetGraph;
};

/**
 * @brief Simple renaming of graphops_t type.
 */
typedef struct graphops_t graphops;

#endif //GRAPHDATA_GRAPHOPS_H
