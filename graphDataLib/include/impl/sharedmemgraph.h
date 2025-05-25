//
// Created by david on 10/21/23.
//

#ifndef GRAPHDATA_SHAREDMEMGRAPH_H
#define GRAPHDATA_SHAREDMEMGRAPH_H

#include <graphData.h>
#include <util/memmgt.h>

/**
 * @brief Metadata structure for shared memory graphs
 */

struct shmemdata_t {
    /**
     * @brief Length of the node array
     */
    size_t nodelen;
    /**
     * @brief Degree of the nodes (number of possible edges coming out
     */
    size_t degree;
    /**
     * @brief Length of the arrays
     *
     * This length is actually the value of nodelen * degree, like the ARRAY implementation
     */
    size_t arraylen;

    key_t nodekey;
    key_t flowkey;
    key_t capkey;

    int nodememid;
    int flowmemid;
    int capmemid;
};

/**
 * Initialize the shared memory graph (nodes, edges, identifiers)
 * @param g graph_t with metadata
 * @return EXIT_SUCCESS if successfully initialized the graph; otherwise, EXIT_FAILURE
 */
int shmemGraphInit(struct graph_t *g);

/**
 * @brief Perform operations to release and deallocate all of the resources used to support this graph.
 * @param g Graph whose resources are to be freed
 * @return EXIT_SUCCESS if all resources freed; otherwise, EXIT_FAILURE
 */
int shmemGraphFree(struct graph_t *g);



#endif //GRAPHDATA_SHAREDMEMGRAPH_H
