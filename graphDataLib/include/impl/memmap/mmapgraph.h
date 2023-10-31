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
 * @brief Initialize a memory-mapped structure a node neighbors array 
 * @param nlen Number/length of the nodes
 * @param conlen number of connnections per
 * @param zeroOut 0 to use the mempry as-is (going to revive a persisted graph, for example); 1 to initialize to all zeroes 
 * @param prot_flags MMap PROT_ flags
 * @param mmap_flags MMap access flags
 * @param nodeFd File descriptor for the backing file.
 * @return Memory address of the file-backed mmap array
 */
void * initMmapInt(size_t nlen, size_t conlen, size_t zeroOut, int prot_flags, int mmap_flags, int nodeFd);

/**
 * @brief Initialize a memory-backed double array
 * @param nlen Number of nodes
 * @param conlen connectivity (number of edges)
 * @param zeroOut 0 to use the memory as-is; 1 to initialize to all zeroes.
 * @param prot_flags MMap PROT_ flags
 * @param mmap_flags MMap access flags
 * @param doubleFd File descriptor for the backing file
 * @return Memroy address of the file-backed mmap array
 */
void * initMMapDouble(size_t nlen, size_t conlen, size_t zeroOut, int prot_flags, int mmap_flags, int doubleFd);

struct mmapdata_t * initMMapMeta(void *addr, int prot_flags, int mmap_flags, int metaFd);

/**
 * Free the metadata memory at the given location
 * @param metaptr 
 * @return 1 if successful, 0 if not
 */
short freeMMapMeta(void **metaptr);

/**
 * @brief Set up a graph with memory-mapped files backing data
 *
 * @param g Graph structure
 * @param lblcount Number of labels to be used--may be zero, depending on the graph domain.
 * @return 1 if successful; 0 if an error
 */
short mmapGraphInit(struct graph_t *g);


/**
 * @brief Perform clearing operations to deallocate the memory-mapped addresses graph internal values and structures.
 * @param g graph_t with array structures to be deallocated
 * @return 1 if successful; otherwise, 0.
 */
short mmapGraphFree(struct graph_t *g);

/**
 * @brief Free the mmap memory for an integer (size_t values) structure at the given address and length
 * @param arrPtr in-memory address address
 * @param memLen number of buckets of the memory segment
 * @return 1 if successful, 0 if not 
 */
short freeMMapInt(void **arrPtr, size_t memLen);

/**
 * @brief Free the mmap memory for a double structure at the given address and length
 * @param dblPtr  in-memory address
 * @param memLen number of buckets of the memory segment
 * @return 1 if successful, 0 if not
 */
short freeMMapDouble(void **dblPtr, size_t memLen);

#endif //GRAPHDATA_MMAPGRAPH_H
20000000000000000000000000000000
