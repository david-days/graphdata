//
// Created by david on 11/11/17.
//

#include<graphinit.h>
#include <stdarg.h>
#include <stdlib.h>
#include <impl/arraygraph.h>
#include <impl/linkgraph.h>
#include <impl/hashgraph.h>

static struct graph_t * basicGraphInit() {
    struct graph_t *g;
    g = (struct graph_t *) malloc(sizeof(struct graph_t));
    if (g == NULL) {
        //something went wrong
        free(g);
    } else {
        //set the initial values to null;
        g->capImpl = NULL;
        g->edgeImpl = NULL;
        g->flowImpl = NULL;
        g->labels = NULL;
        g->metaImpl = NULL;
        g->nodeImpl = NULL;
    }
    return g;
}

/**
 * @brief Create a dimension structure containing the given values in order (x, y, z, etc)
 *
 * This is used to initialize (and contain) the ordered size of the spatial dimensions given.  Consumers are responsible
 * for calling free() on the structure, or passing it to a standard clearing function
 *
 * @param dimsz Number of dimensions
 * @param ... Additional dimension values passed
 * @return Pointer to a dimensions_t structure that holds the count and values of the structure, or a null pointer
 * if there is a problem allocating the memory.
 */
struct dimensions_t * createDimensions(size_t dimsz, ...) {
    va_list dlist;
    struct dimensions_t *dims = (struct dimensions_t *)malloc(sizeof(struct dimensions_t));
    size_t *dimarr = (size_t *)malloc(dimsz*sizeof(size_t));
    if (NULL != dims && NULL != dimarr) {
        va_start(dlist, dimsz);
        size_t *dp = dimarr;
        for (size_t i=0; i<dimsz;i++) {
            *(dp+i) = va_arg(dlist, size_t);
        }
        dims->dimcount = dimsz;
        dims->dimarr = dimarr;
        va_end(dlist);
    }
    else {
        //something went wrong--free up the struct if necessary
        if (dims != NULL)
            free(dims);
        return NULL;
    }
    return dims;
}

/**
 * @brief Raw initializer for graphops_t structure with all NULL interior pointers
 *
 * Consumers of this object are responsible for calling free() on the pointer when finished, or passing it to
 * a standard cleanup function.
 *
 * @return Pointer to a graphops structure, or NULL if there was a problem with memory allocation
 */
struct graphops_t * initGraphops() {
    struct graphops_t *gops = (struct graphops_t *) malloc(sizeof(struct graphops_t));
    if (NULL != gops) {
        gops->g = NULL;
        gops->addEdge = NULL;
        gops->addNode = NULL;
        gops->edgeCount = NULL;
        gops->edgePath = NULL;
        gops->edgePath = NULL;
        gops->getEdge = NULL;
        gops->getEdges = NULL;
        gops->getNeighbors = NULL;
        gops->getNode = NULL;
        gops->nodeCount = NULL;
        gops->nodePath = NULL;
        gops->resetGraph = NULL;
        gops->setCapacity = NULL;
    }
    return gops;
}

/**
 * @brief Initialize a general graph structure.
 *
 * Create a generic graph structure, based on the parameters passed.  The underlying structure will be created in accordance
 * with the combinations of GRAPHTYPE and GRAPHIMPL values.  For GRAPHIMPL = ARRAY, the dimensions value is required and must not be NULL.
 * For other implementations, passing a valid dimensions_t structure is optional (usage dependent).
 *
 * The returned graph structure pointer will have a NULL pointer for g->labels.
 *
 * @param gtype Type of graph (DIRECTED or UNDIRECTED).
 * @param impltype Type of implementation requested.  Use GENERIC for a general graph.
 * @param dims Dimensions for a spatial graph.  Required for ARRAY implementations, NULL or optional for all others.
 * @return Pointer to the graph structure created, if parameters are valid.  NULL pointer returned if the parameters are invalid or there is
 * a problem allocating the memory required.
 */
struct graph_t * initGraph(enum GRAPHTYPE gtype, enum GRAPHIMPL impltype, struct dimensions_t *dims) {
    struct graph_t *g = basicGraphInit();

    return g;
}

/**
 * @brief Initialize a graph structure primed for usage in graph-label operations.
 *
 * Creates a graph structure, based on the parameters passed, with the g->labels structure filled accordingly.  For GRAPHIMPL = ARRAY, the
 * dimensions value will be created, and the label structure will contain the values for the defined label nodes.  For all others, the
 * label array will be initialized with 0, which may or may not be valid identifiers.
 *
 * @param gtype Type of graph (DIRECTED or UNDIRECTED)
 * @param impltype Type of implementation required.  use Generic for general graph.
 * @param lblcount Number of label values to be used.
 * @param dims Dimensions for spatial graph.  Required for ARRAY implementations, NULL or optional for all others.
 * @return Pointer to the graph structure created, if the parameters are valid.  NULL pointer returned if the parameters are invalid
 * or there is a problem allocating the memory required.
 */
struct graph_t * initLabelGraph(enum GRAPHTYPE gtype, enum GRAPHIMPL impltype, size_t lblcount, struct dimensions_t *dims) {
    struct graph_t *g = basicGraphInit();

    return g;
}