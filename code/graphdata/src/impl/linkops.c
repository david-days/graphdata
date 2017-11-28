/**
 * @brief Operations for a linked-list graph structure.
 *
 * This implementation is for a linked-list graph structure that uses the minimum necessary memory to store
 * vertex and edge data for a graph.  The intent is to support the most general graph usage, wherein the user of the
 * graph structure may not be able to determine exact relationship before graph creation.
 *
 * In this first version, the operations are implemented around a double-linked list structure, with node and edge walking
 * as necessary to find the appropriate relationships.  Future versions will attempt to add more nuanced and intelligent
 * layouts in order to speed up operations.
 */

#include <impl/linkops.h>


//Read functions to extract data
/**
 * @brief Function pointer definition for getting the node count;
 * @param g Graph structure in question
 * @return Count of nodes, if graph is not null; otherwise, return 0
 */
size_t linkNodeCount(struct graph_t *g) {

}

/**
 * @brief Function pointer to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
size_t linkEdgeCount(struct graph_t *g) {

}

/**
 * @brief Function pointer to retrieve a node structure reference.
 *
 * Implementation-specific on whether this
 * structure is part of the original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, pointer to NULL
 */
struct node_t * linkGetNode(const size_t *nodeid, const struct graph_t *g) {

}

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
struct edge_t * linkGetEdge(const size_t *u, const size_t *v, const struct graph_t *g) {

}

/**
 * @brief Function pointer to retrieve linked-list of nodes that are currently defined as neighbors to the given node.
 *
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of node references, starting with the given node, if found; otherwise, pointer to NULL.
 */
struct node_t * linkGetNeighbors(const size_t *nodeid, const struct graph_t *g) {

}

/**
 * @brief Function pointer to retrieve linked-list of edges from a given node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of edges starting from the given node, if found; otherwise, pointer to NULL.
 */
struct edge_t * linkGetEdges(const size_t *nodeid, const struct graph_t *g) {

}

/**
 * @brief Function pointer to retrieve linked-list path of nodes from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of nodes, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
struct node_t * linkGetNodePath(const size_t *uid, const size_t *vid, const struct graph_t *g) {

}

/**
 * @brief Function pointer to retrieve linked-list of path edges from start to destination, beginning with the starting node.
 * Returned linked-list is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of edges, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
struct edge_t * linkGetEdgePath(const size_t *uid, const size_t *vid, const struct graph_t *g) {

}

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
int linkGetCapacity(const size_t *uid, const size_t *vid, double *cap, const struct graph_t *g) {

}

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
int linkGetFlow(const size_t *uid, const size_t *vid, double *flow, const struct graph_t *g) {

}

//Write functions to modify graph
/**
 * @brief Function pointer to add a node to a given graph.
 * Not all implementations may use this (for example, fixed-size graphs such as spatial ARRAY implementations).
 * @param nodeid Node identifier to be added
 * @param g Graph structure to add the node
 * @return 0 if there was an error, 1 if the node was successfully added
 */
int linkAddNode(const size_t *nodeid, struct graph_t *g) {

}

/**
 * @brief Remove a node from the graph.
 *
 * Not all implementations will allow removal of nodes.
 * @param nodeid Node id to be added.
 * @param g Graph structure in question
 * @return 0 if there was an error (node already exists or outside the bounds of the implementation); otherwise, 1 if successful.
 */
int linkRemoveNode(const size_t *nodeid, struct graph_t *g) {

}

/**
 * @brief Function pointer to add an edge to a given graph.
 * Not all implementations may use this (for example, fixed-size ARRAY implementations representing a set domain of nodes and relationships).
 * @param uid identifer for start of edge
 * @param vid identifier for end of edge
 * @param cap capacity value to be assigned
 * @param g graph structure in question
 * @return 0 if there was an error; 1 if the edge was successfully added.
 */
int linkAddEdge(const size_t *uid, const size_t *vid, double *cap, struct graph_t *g) {

}

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
int linkRemoveEdge(const size_t *uid, const size_t *vid, struct graph_t *g) {

}

/**
 * @brief Function pointer to set the capacity (cost, weight, etc.) of an edge in the given graph.
 * @param uid identifier of the edge start
 * @param vid identifier of the edge ending.
 * @param cap capacity value to be set
 * @param g Graph structure in question
 * @return 0 if there was an error; 1 if the capacity was successfully set
 */
int linkSetCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {

}

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
int linkAddCapacity(const size_t *uid, const size_t *vid, const double *cap, struct graph_t *g) {

}

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
int linkSetFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {

}

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
int linkAddFlow(const size_t *uid, const size_t *vid, const double *flow, struct graph_t *g) {

}

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
int linkResetGraph(struct graph_t *g, void *args, void *callback(void)) {

}

