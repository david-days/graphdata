/**
 * @brief Private graph structure helpers.
 *
 * Caller-owned create, clone, and destroy operations are declared in graphCrud.h,
 * which this header includes. The declarations below are for initGraph, the
 * backends, and in-tree tests. This header is not installed.
 */

#ifndef GRAPHDATA_CRUDOPS_H
#define GRAPHDATA_CRUDOPS_H

#include <graphCrud.h>
#include <stdlib.h>

/**
 * @brief Selector for graph directionality
 */
#define DIRSELECT DIRECTED | UNDIRECTED
/**
 * @brief Selector for underlying structure
 */
#define IMPLSELECT ARRAY | LINKED | HASHED | SHARED_MEM | SHARED_MMAP
/**
 * @brief Selector for domain
 */
#define DOMAINSELECT GENERIC | SPATIAL
/**
 * @brief Selector for labels
 */
#define LABELSELECT LABELED | UNLABELED
/**
 * @brief Check for no flags passed--return default
 */
#define DEFAULTSELECT 0
/**
 * @brief Default graph type returned, if no parameters passed
 */
#define DEFAULTGRAPH UNDIRECTED | LINKED | GENERIC | UNLABELED
/**
 * @brief readonly graph data--used primarily for shared graph structures (shmem and mmap)
 */
#define READONLY RD_ONLY

/**
 * @brief Parse the flag values passed, and write the evaluation into the separate references
 *
 * Parses out the separate possibilities for the flags. If a selector group is empty, its default
 * is applied (UNDIRECTED, LINKED, UNLABELED, GENERIC). Exactly one implementation bit from
 * {ARRAY, LINKED, HASHED, SHARED_MEM, SHARED_MMAP} may be set (none → LINKED). Label and domain
 * groups likewise allow at most one bit each (LABELED vs UNLABELED, GENERIC vs SPATIAL).
 * On failure, outputs and *tflags are left unchanged.
 *
 * @param tflags Flag values passed to be evaluated (cleaned combination written back on success)
 * @param dirflag Directionality result of the operation
 * @param impflag Implementation type result
 * @param lblflag Label type result
 * @param domflag Domain type result
 * @param roflag Read only type result
 * @return GRAPH_OK if parsing succeeded; GRAPH_ERR on null args or conflicting implementation flags
 */
int parseTypeFlags(enum GRAPHDOMAIN *tflags, enum GRAPHDOMAIN *dirflag, enum GRAPHDOMAIN *impflag,
                   enum GRAPHDOMAIN *lblflag, enum GRAPHDOMAIN *domflag, enum GRAPHDOMAIN *roflag);


//Creation operations

/**
 * @brief Utility method to create and preset graph structure.
 * @return A pointer to a graph_t structure, if successful; otherwise, NULL
 */
struct graph_t * basicGraphInit();

/**
 * @brief Create a raw label structure of the given size
 * @param lblcount Number of labels required
 * @return Label structure properly initialized with a size_t array of the given size, if successful; otherwise, a NULL
 * pointer.
 */
struct labels_t * initLabels(size_t lblcount);

/**
 * @brief Raw initializer for graphops_t structure
 *
 * Release the result with destroyGraphops.
 *
 * @return Pointer to a graphOps structure, or NULL if there was a problem with memory allocation
 */
struct graphops_t * initGraphops();

/**
 * @brief Build a caller-owned node-id list by walking with a foreach implementation.
 *
 * Backend listNodes functions call this so the list and the walk cannot drift.
 * An empty walk returns NULL. If a node allocation fails, any partial chain is
 * freed and the result is NULL.
 *
 * @param g Graph to enumerate
 * @param walk Backend foreach implementation
 * @return caller-owned node chain, or NULL when empty or on allocation failure
 */
struct node_t *listNodesByWalk(const struct graph_t *g, funcForeachNode walk);

#endif //GRAPHDATA_CRUDOPS_H
