//
// Created by david on 11/12/17.
//

#ifndef GRAPHDATA_GRAPHOPS_H
#define GRAPHDATA_GRAPHOPS_H

#include <graphData.h>

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
 * @brief Function pointer to retrieve a caller-owned copy of a node.
 *
 * The snapshot carries nodeid only. Mutating it does not change the live graph.
 * Release it with destroyNodes. Returns NULL if the node is not present.
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return caller-owned node copy, if found; otherwise NULL
 */
typedef struct node_t * (*funcGetNode)(const size_t *nodeid, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve a caller-owned copy of an edge.
 *
 * The snapshot carries u, v, cap, and flow. Mutating it does not change the
 * live edge. Release it with destroyEdges. Returns NULL if the edge is absent.
 * @param u nodeid of the starting edge.
 * @param v nodeid of the ending edge.
 * @param g Graph structure in question
 * @return caller-owned edge copy, if found; otherwise NULL
 */
typedef struct edge_t * (*funcGetEdge)(const size_t *u, const size_t *v, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Caller-owned list of neighbor ids. The queried node is not included.
 * Release the chain with destroyNodes. NULL when the node is absent or has no neighbors.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return caller-owned neighbor chain, or NULL
 */
typedef struct node_t * (*funcGetNeighbors)(const size_t *nodeid, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve linked-list of edges from a given node.
 * Caller-owned copies of the node's edges. Release the chain with destroyEdges.
 * NULL when the node is absent or has no edges.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return caller-owned edge chain, or NULL
 */
typedef struct edge_t * (*funcGetEdges)(const size_t *nodeid, const struct graph_t *g);

/**
 * @brief Function pointer to retrieve the current capacity value for a given edge.
 *
 * For implementations that support this, the capacity value will be written to the *cap parameter.
 *
 * @param uid Edge start identifier
 * @param vid Edge end identifier
 * @param cap Capacity value pointer to store the value
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
typedef int (*funcGetFlow)(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g);

//Write functions to modify graph
/**
 * @brief Function pointer to add a node to a given graph.
 * Not all implementations may use this (for example, fixed-size graphs such as spatial ARRAY implementations).
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
typedef int (*funcAddNode)(const size_t *nodeid, struct graph_t *g);

/**
 * @brief Remove a node from the graph.
 *
 * Not all implementations will allow removal of nodes.
 * @param nodeid Node id to be removed.
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
typedef int (*funcRemoveNode)(const size_t *nodeid, struct graph_t *g);

/**
 * @brief Function pointer to add an edge to a given graph.
 * ARRAY and the shared backends return GRAPH_ERR when every adjacency slot is full.
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
typedef int (*funcRemoveEdge)(const size_t *uid, const size_t *vid, struct graph_t *g);

/**
 * @brief Function pointer to set the capacity (cost, weight, etc.) of an edge in the given graph.
 * @param uid identifier of the edge start
 * @param vid identifier of the edge ending.
 * @param cap capacity value to be set
 * @param g Graph structure in question
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
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
 * @return GRAPH_OK on success; GRAPH_ERR on failure.
 */
typedef int (*funcResetGraph)(struct graph_t *g, void *args, void (*callback)(void));

/**
 * @brief Visitor for node enumeration.
 *
 * @param nodeid Present node id. The pointer is valid only for this call.
 * @param ctx Caller context passed through foreachNode.
 * @return GRAPH_OK to continue; GRAPH_ERR to stop the walk.
 */
typedef int (*funcNodeVisitor)(const size_t *nodeid, void *ctx);

/**
 * @brief Visit each present node id once.
 *
 * Order is backend-defined and matches listNodes on an unchanged graph.
 * The visitor must not add or remove nodes.
 * @param g Graph to walk
 * @param visit Callback invoked once per present node id
 * @param ctx Context forwarded to visit
 * @return GRAPH_OK if every present node was visited, including an empty graph;
 *         GRAPH_ERR if visit stopped early, or if g or visit is NULL
 */
typedef int (*funcForeachNode)(const struct graph_t *g, funcNodeVisitor visit, void *ctx);

/**
 * @brief Caller-owned list of present node ids.
 *
 * Each node_t carries nodeid only (attrs and edges are NULL). Release the
 * chain with destroyNodes. An empty graph returns NULL. NULL when nodeCount
 * is greater than zero means allocation failed. Destroying the list does not
 * remove live nodes. Order matches foreachNode on an unchanged graph.
 * @param g Graph to enumerate
 * @return caller-owned node chain, or NULL if the graph is empty or allocation failed
 */
typedef struct node_t *(*funcListNodes)(const struct graph_t *g);


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


    /**
     * @brief Reset the graph to an initial state, according to implementation logic
     * Means of resetting the graph to an initial state without having to re-implement cleanup logic
     * or creating a new graph structure.
     */
    funcResetGraph resetGraph;

    /**
     * @brief Caller-owned list of present node ids. Release with destroyNodes.
     */
    funcListNodes listNodes;

    /**
     * @brief Visit each present node id once.
     */
    funcForeachNode foreachNode;
};

#endif //GRAPHDATA_GRAPHOPS_H
