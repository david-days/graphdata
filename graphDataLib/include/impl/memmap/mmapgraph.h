//
// Created by david on 10/21/23.
//


#ifndef GRAPHDATA_MMAPGRAPH_H
#define GRAPHDATA_MMAPGRAPH_H

#include <stdlib.h>
#include <graphData.h>

/**
 * @brief Metadata structure for memory-mapped file graphs
 */
struct mmapdata_t {
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

    /**
     * @brief Length of the arrays
     *
     * This length is actually nodecount * degree for the FILE implementation
     */
    size_t mmaplen;
    
    /**
     * @brief base path for the memory-map files
     */
    char *memfileBase;
    
    /**
     * @brief path to the node mmap file
     */
    char *nodeFile;
    
    /**
     * @brief path to the label mmap file
     */
    char *labelFile;
    
    /**
     * @brief path to the metadata structure mmap file (this structure)
     */
    char *metaFile;
    
    /**
     * @brief memory mapped file protection/access flags (PROT_READ, PROT_WRITE, etc)
     */
    int mmap_prot;
    /**
     * @brief memory-mapped file access flags.
     */
    int mmap_flags;
    
    /**
     * @brief Address of the graph structure.  May be NULL
     */
    void *graphAddress;
};

/**
 * @brief Set up a graph with memory-mapped files backing data
 *
 * @param g Graph structure
 * @param lblcount Number of labels to be used--may be zero, depending on the graph domain.
 * @return 1 if successful; 0 if an error
 */
int mmapGraphInit(struct graph_t *g);


/**
 * @brief Perform clearing operations to deallocate the memory-mapped addresses graph internal values and structures.
 * @param g graph_t with array structures to be deallocated
 * @return 1 if successful; otherwise, 0.
 */
int mmapGraphFree(struct graph_t *g);


#endif //GRAPHDATA_MMAPGRAPH_H
