//
// Created by david on 10/21/23.
//

#ifndef GRAPHDATA_SHAREDMMAPGRAPH_H
#define GRAPHDATA_SHAREDMMAPGRAPH_H

#include <graphData.h>
#include <util/memmgt.h>

/**
 * Metadata for memory-mapped graphs.
 *
 * The first four fields mirror arraydata_t so mmap ops can reuse
 * the array implementation helpers safely.
 */
struct shmmapdata_t {
    /**
     * length of the node array
     */
    size_t nodelen;
    /**
     * length of the edge array (matches ARRAY: same as nodelen)
     */
    size_t edgelen;
    /**
     * degree of nodes (number of possible edges for each node)
     */
    size_t degree;
    /**
     * Length of the arrays (nodelen * degree)
     */
    size_t arraylen;

    /**
     * @brief read-only flag for this graph
     */
    enum GRAPHDOMAIN roflag;

    char *nodefile;
    char *flowfile;
    char *capfile;

};

/**
 * Initialize a memory-mapped graph structure
 * @param g graph_t with metadata
 * @return GRAPH_OK if successfully initialize; otherwise, GRAPH_ERR
 */
int shmmapGraphInit(struct graph_t *g);

/**
 * PROVISIONAL: Attach a read-only view of an existing writable SHARED_MMAP graph.
 *
 * Re-maps the source file paths with PROT_READ. In-process helper for policy
 * testing — not the eventual multi-process reader/discovery API.
 *
 * @param g destination graph (basic fields already set; dims shared with src)
 * @param src writable source graph
 * @return GRAPH_OK if attached; otherwise GRAPH_ERR
 */
int shmmapGraphAttachReadonly(struct graph_t *g, const struct graph_t *src);

/**
 * Perform the operations to clean up and free the graph structure
 * @param g Graph structure to clean up
 * @return GRAPH_OK if all resources freed; othewise, GRAPH_ERR
 */
int shmmapGraphFree(struct graph_t *g);

#endif //GRAPHDATA_SHAREDMMAPGRAPH_H
