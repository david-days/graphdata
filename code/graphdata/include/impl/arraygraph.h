/**
 * @brief Array implementation of the graph structure for fixed-size, fixed-degree, UNDIRECTED graphs.
 *
 * The array implementation of the graph structure is based on an UNDIRECTED, fixed-size and
 * fixed maximum degree for the nodes.
 *
 * Because the graph is UNDIRECTED, the start of each edge will actually be minNode(u,v) and the
 * end of each edge will be maxNode(u,v).  Thus, node 0 can connect "upwards", but no higher nodes
 * can connect down to node 0.  In all the underlying operations, having 0 in a node reference value
 * indicates an unused edge.
 */

#ifndef GRAPHDATA_ARRAYGRAPH_H
#define GRAPHDATA_ARRAYGRAPH_H

#include <graphdata.h>

/**
 * @brief Metadata structure for array graphs
 */
struct arraydata_t {
    /**
     * @brief Length of the node array
     */
    size_t nodelen;
    /**
     * @brief Length of the edge array
     */
    size_t edgelen;
    /**
     * @brief Degree of the nodes--the number of (possible) edges coming out of each node.
     */
    size_t degree;


};

/**
 * @brief Set up a graph with array backing data
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @return 1 if successful; 0 if an error
 */
int arrayGraphInit(enum GRAPHTYPE gtype, struct graph_t *g);


/**
 * @brief Set up a graph with array and backing data, adding on label connections as necessary.
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @param lblcount Number of label nodes to be added
 * @return 1 if successful; 0 of an error.
 */
int arrayGraphLabelInit(enum GRAPHTYPE gtype, struct graph_t *g, size_t lblcount);

/**
 * @brief Perform clearing operations to deallocate the array graph internal values and structures.
 * @param g graph_t with array structures to be deallocated
 * @return 1 if successful; otherwise, 0.
 */
int arrayGraphFree(struct graph_t *g);

#endif //GRAPHDATA_ARRAYGRAPH_H
