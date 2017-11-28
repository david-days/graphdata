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
typedef size_t (*funcNodeCount)(struct graph_t *g);

/**
 * @brief Function pointer to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
typedef size_t (*funcEdgeCount)(struct graph_t *g);

/**
 * @brief Function pointer to retrieve a node structure reference.
 *
 * Implementation-specific on whether this
 * structure is part of the original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, pointer to NULL
 */
typedef struct node_t * (*funcGetNode)(const size_t *nodeid, const struct graph_t *g);

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
typedef struct edge_t * (*funcGetEdge)(const size_t *u, const size_t *v, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of node references, starting with the given node, if found; otherwise, pointer to NULL.
 */
typedef struct node_t * (*funcGetNeighbors)(const size_t *nodeid, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve linked-list of edges from a given node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of edges starting from the given node, if found; otherwise, pointer to NULL.
 */
typedef struct edge_t * (*funcGetEdges)(const size_t *nodeid, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve linked-list path of nodes from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of nodes, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
typedef struct node_t * (*funcGetNodePath)(const size_t *uid, const size_t *vid, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve linked-list of path edges from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of edges, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
typedef struct edge_t * (*funcGetEdgePath)(const size_t *uid, const size_t *vid, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve the current capacity value for a given edge.
 *
 * For implementations that support this, the capacity value will be written to the *cap parameter.
 *
 * @param uid Edge start identifier
 * @param vid Edge end identifier
 * @param cap Capacity value pointer to store the value
 * @param g Graph structure in question
 * @return 0 if there was a problem retrieving the value (such as the edge not existing); otherwise, 1 for a successful
 * retrieval
 */
typedef int (*funcGetCapacity)(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve the current flow value for a given edge.
 *
 * For implementations that support this, the flow value will be written to the *flow parameter.
 *
 * @param uid Edge start identifier
 * @param vid Edge end identifier
 * @param flow Flow value pointer to store the result
 * @param g Graph structure in question
 * @return 0 if there was a problem retrieving the value (such as the edge not existing); otherwise, 1 for a successful
 * retrieval
 */
typedef int (*funcGetFlow)(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g);

//Write functions to modify graph
/**
 * @brief Function pointer to add a node to a given graph.
 * Not all implementations may use this (for example, fixed-size graphs such as spatial ARRAY implementations).
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return 0 if there was an error, 1 if the node was successfully added
 */
typedef int (*funcAddNode)(const size_t *nodeid, struct graph_t *g);

/**
 * @brief Remove a node from the graph.
 *
 * Not all implementations will allow removal of nodes.
 * @param nodeid Node id to be added.
 * @param g Graph structure in question
 * @return 0 if there was an error (node already exists or outside the bounds of the implementation); otherwise, 1 if successful.
 */
typedef int (*funcRemoveNode)(const size_t *nodeid, struct graph_t *g);

/**
 * @brief Function pointer to add an edge to a given graph.
 * Not all implementations may use this (for example, fixed-size ARRAY implementations representing a set domain of nodes and relationships).
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return 0 if there was an error; 1 if the edge was successfully added.
 */
typedef int (*funcAddEdge)(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g);

/**
 * @brief Function pointer to remove an edge from the given graph.
 *
 * Allows implementations to remove a given edge from the graph, if necessary.
 *
 * @param uid Identifier for the edge start
 * @param vid Identifier for the edge end.
 * @param g Graph structure in question
 * @return 0 if there was an error (e.g. the edge was not found); otherwise, 1 if the edge was removed.
 */
typedef int (*funcRemoveEdge)(const size_t *uid, const size_t *vid, struct graph_t *g);

/**
 * @brief Function pointer to set the capacity (cost, weight, etc.) of an edge in the given graph.
 * @param uid identifier of the edge start
 * @param vid identifier of the edge ending.
 * @param cap capacity value to be set
 * @param g Graph structure in question
 * @return 0 if there was an error; 1 if the capacity was successfully set
 */
typedef int (*funcSetCapacity)(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g);

/**
 * @brief Function to add (adjust) the capacity for an edge by a given amount.
 *
 * For implementations that require it, this function allows the altering of a specified edge capacity by the given amount.
 *
 * @param uid Identifier of the edge start
 * @param vid Identifier of the edge end.
 * @param cap Value to adjust the capacity
 * @param g Graph structure in question
 * @return 0 if there was an error (edge not found, for example); 1 of capacity was successfully adjusted
 */
typedef int (*funcAddCapacity)(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g);

/**
 * @brief Function to set the flow value for an edge (amount of capacity currently "used")
 *
 * For implementations that require it, this allows tracking of flow values that are used within the graph.
 *
 * @param uid Identifier of the edge start.
 * @param vid Identifier of the edge end.
 * @param flow Value to be set for the flow.
 * @param g Graph structure in question
 * @return 0 of there was an error (edge not found, for example); otherwise, 1 if the flow value as successfully set.
 */
typedef int (*funcSetFlow)(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g);

/**
 * @brief Function to adjust the flow value of a given edge.
 *
 * For implementations that require it, this allows ajustment of the amount of capacity that is being "used".
 *
 * @param uid Identifier of the edge start
 * @param vid Identifier of the edge end.
 * @param flow The value to be added to adjust the flow value.
 * @param g The graph structure in question
 * @return 0 if there was an error (such as the edge not found); otherwise, 1 if the flow value was successfully adjusted.
 */
typedef int (*funcAddFlow)(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g);

/**
 * @brief Function pointer to "reset" the graph according to the given argument pointer.
 *
 * For some implementations, it is more efficient to reuse the existing graph structure and perform a "zero-out"
 * of the data, rather than rebuilding from scratch.  This function pointer provides that option.
 * @param g Graph structure to be zeroed or modified according to reset logic
 * @param args Arguments to be used in the reset process, if necessary
 * @param callback Callback to be executed when graph has been reset.
 * @return 0 if there was an error during the reset; 1 if the reset completed;
 */
typedef int (*funcResetGraph)(struct graph_t *g, void *args, void (*callback)(void));


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
    struct graph_t *g;

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
    /**
     * @brief Retrieve the capacity value for the given edge
     */
    funcGetCapacity getCapacity;
    /**
     * @brief Retrieve the flow value for a given edge
     */
    funcGetFlow getFlow;

    //Modifiers
    /**
     * @brief Add a node to the graph.
     * Expected to be NULL (no implementation) for fixed-size graph structures.
     */
    funcAddNode addNode;
    /**
     * @brief Remove a node from the graph.
     * Expected to be NULL or NOOP for fixed-size graph structures.
     */
    funcRemoveNode removeNode;
    /**
     * @brief Add an edge to the graph.
     * Expected to be NULL (no implementation) for fixed-size graph structures.
     */
    funcAddEdge addEdge;
    /**
     * @brief Remove an edge from the graph.
     */
    funcRemoveEdge removeEdge;
    /**
     * Set the capacity for a given edge.
     */
    funcSetCapacity setCapacity;

    /**
     * Adjust the capacity of an edge
     */
    funcAddCapacity  addCapacity;

    /**
     * Set the flow value of an edge.
     */
    funcSetFlow setFlow;

    /**
     * Adjust the flow value of an edge
     */
    funcAddFlow addFlow;

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

#endif //GRAPHDATA_GRAPHOPS_H
