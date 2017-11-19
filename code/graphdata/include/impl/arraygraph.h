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

int arrayGraphInit(enum GRAPHTYPE gtype, struct graph_t *g);

int arrayGraphLabelInit(enum GRAPHTYPE gtype, struct graph_t *g, size_t lblcount);

#endif //GRAPHDATA_ARRAYGRAPH_H
