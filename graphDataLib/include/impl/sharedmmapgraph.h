//
// Created by david on 10/21/23.
//

#ifndef GRAPHDATA_SHAREDMMAPGRAPH_H
#define GRAPHDATA_SHAREDMMAPGRAPH_H

#include <graphData.h>
#include <util/memmgt.h>

struct shmmapdata_t {
    /**
     * @brief read-only flag for this graph
     */
    enum GRAPHDOMAIN roflag;

    /**
     * length of the node array
     */
    size_t nodelen;
    /**
     * degree of nodes (number of possible edges for each node)
     */
    size_t degree;
    /**
     * Length of the arrays
     *
     * This length is actually the value of nodelen * degree
     */
    size_t arraylen;

    char *nodefile;
    char *flowfile;
    char *capfile;

};

/**
 * Initialize a memory-mapped graph structure
 * @param g graph_t with metadata
 * @return EXIT_SUCCESS if successfully initialize; otherwise, EXIT_FAILURE
 */
int shmmapGraphInit(struct graph_t *g);

/**
 * Perform the operations to clean up and free the graph structure
 * @param g Graph structure to clean up
 * @return EXIT_SUCCESS if all resources freed; othewise, EXIT_FAILURE
 */
int shmmapGraphFree(struct graph_t *g);

#endif //GRAPHDATA_SHAREDMMAPGRAPH_H
