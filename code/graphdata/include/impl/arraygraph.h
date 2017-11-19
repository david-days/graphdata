//
// Created by david on 11/18/17.
//

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
     * @brief Connectivity count--length of the second dimension of the arrays
     */
    size_t conncount;


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

#endif //GRAPHDATA_ARRAYGRAPH_H
