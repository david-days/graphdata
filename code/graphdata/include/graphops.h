//
// Created by david on 11/12/17.
//

#ifndef GRAPHDATA_GRAPHOPS_H
#define GRAPHDATA_GRAPHOPS_H

#include <graphdata.h>

//Read functions to extract data
/**
 * Function pointer definition for getting the node count;
 * @param g Graph structure in question
 * @return Count of nodes, if graph is not null; otherwise, return 0
 */
typedef size_t (*funcNodeCount)(graph *g);
/**
 * Function pointer to extract count of edges
 * @param g Graph structure in question
 * @return Count of edges, if graph is not null; otherwise, return 0
 */
typedef size_t (*funcEdgeCount)(graph *g);
/**
 * Function pointer to retrieve a node structure reference.  Implementation-specific on whether this
 * structure is part of the original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param nodeid Identifier of the node to be retrieved
 * @param g Graph structure in question
 * @return pointer to the node structure, if found; otherwise, pointer to NULL
 */
typedef node * (*funcGetNode)(const size_t *nodeid, const graph *g);
/**
 * Function pointer to retrieve a edge structure reference.  Implementation-specific on wheter this structure is part of the
 * original graph (LINK-based implementations) or must use free() when usage is finished (ARRAY-based).
 * @param u nodeid of the starting edge.
 * @param v nodeid of the ending edge.
 * @param g Graph structure in question
 * @return pointer to the edge structure, if found; otherwise, pointer to NULL.
 */
typedef edge * (*funcGetEdge)(const size_t *u, const size_t *v, const graph *g);
/**
 * Function pointer to retrieve linked-list of nodes that are currently defined as neighbors to the given node.  Returned linked-list is distinct from
 * the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of node references, starting with the given node, if found; otherwise, pointer to NULL.
 */
typedef node * (*funcGetNeighbors)(const size_t *nodeid, const graph *g);
/**
 * Function pointer to retrieve linked-list of edges from a given node.  Returned linked-list is distinct from
 * the graph structure, and consumers must use free() when finished.
 * @param nodeid Identifier of the node in question
 * @param g Graph structure in question
 * @return linked-list of edges starting from the given node, if found; otherwise, pointer to NULL.
 */
typedef edge * (*funcGetEdges)(const size_t *nodeid, const graph *g);
/**
 * Function pointer to retrieve linked-list path of nodes from start to destination, beginning with the starting node.  Returned linked-list
 * is distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of nodes, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
typedef node * (funcGetNodePath)(const size_t *uid, const size_t *vid, const graph *g);
/**
 * Function pointer to retrieve linked-list of path edges from start to destination, beginning with the starting node.  Returned linked-list is
 * distinct from the graph structure, and consumers must use free() when finished.
 * @param uid Path start identifier
 * @param vid Path end identifier
 * @param g Graph in question
 * @return Linked-list of edges, starting from uid and ending at vid, if found; otherwise, pointer to NULL.
 */
typedef edge * (funcGetEdgePath)(const size_t *uid, const size_t *vid, const graph *g);

//Write functions to modify graph
/**
 *
 * @param nodeid
 * @param g
 * @return
 */
typedef int (*funcAddNode)(const size_t *nodeid, graph *g);
/**
 *
 * @param uid
 * @param vid
 * @param cap
 * @param g
 * @return
 */
typedef int (*funcAddEdge)(const size_t *uid, const size_t *vid, double *cap, graph *g);
/**
 *
 * @param uid
 * @param vid
 * @param cap
 * @param g
 * @return
 */
typedef int (*funcSetCapacity)(const size_t *uid, const size_t *vid, const double *cap, graph *g);



struct graphops_t {
    //Graph structure
    graph *g;

    //Getters
    funcNodeCount nodeCount;
    funcEdgeCount edgeCount;
    funcGetNode getNode;
    funcGetEdge getEdge;
    funcGetNeighbors getNeighbors;
    funcGetEdges getEdges;

    //Modifiers
    funcAddNode addNode;
    funcAddEdge addEdge;
    funcSetCapacity setCapacity;

    //Traverse functions
    funcGetNodePath nodePath;
    funcGetEdgePath edgePath;
};

typedef struct graphops_t graphops;

#endif //GRAPHDATA_GRAPHOPS_H
