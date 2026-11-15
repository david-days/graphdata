//
// Created by david on 10/21/23.
//

#ifndef GRAPHDATA_SHAREDMEMGRAPH_H
#define GRAPHDATA_SHAREDMEMGRAPH_H

#include <graphData.h>
#include <util/memmgt.h>
#include <sys/types.h>

/**
 * @brief Metadata structure for shared memory graphs
 *
 * The first four fields mirror arraydata_t so shared-memory ops can reuse
 * the array implementation helpers safely.
 */
struct shmemdata_t {
    /**
     * @brief Length of the node array
     */
    size_t nodelen;
    /**
     * @brief Length of the edge array (matches ARRAY: same as nodelen)
     */
    size_t edgelen;
    /**
     * @brief Degree of the nodes (number of possible edges coming out)
     */
    size_t degree;
    /**
     * @brief Length of the arrays (nodelen * degree)
     */
    size_t arraylen;

    /**
     * @brief read-only flag for this graph
     */
    enum GRAPHDOMAIN roflag;

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
 * @return GRAPH_OK if successfully initialized the graph; otherwise, GRAPH_ERR
 */
int shmemGraphInit(struct graph_t *g);

/**
 * PROVISIONAL: Attach a read-only view of an existing writable SHARED_MEM graph.
 *
 * Copies SysV keys from src and attaches with SHM_RDONLY. This is an in-process
 * helper for policy testing — not the eventual multi-process reader/discovery API.
 *
 * @param g destination graph (basic fields already set; dims shared with src)
 * @param src writable source graph
 * @return GRAPH_OK if attached; otherwise GRAPH_ERR
 */
int shmemGraphAttachReadonly(struct graph_t *g, const struct graph_t *src);

/**
 * @brief Perform operations to release and deallocate all of the resources used to support this graph.
 * @param g Graph whose resources are to be freed
 * @return GRAPH_OK if all resources freed; otherwise, GRAPH_ERR
 */
int shmemGraphFree(struct graph_t *g);



#endif //GRAPHDATA_SHAREDMEMGRAPH_H
