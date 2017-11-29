//
// Created by david on 11/27/17.
//

#include <util/crudops.h>
#include <stdarg.h>

/**
 * @brief Utility method to create and preset graph structure.
 * @return A pointer to a graph_t structure.
 */
struct graph_t * basicGraphInit() {
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
 * @brief Create a raw label structure of the given size
 * @param lblcount Number of labels required
 * @return Label structure properly initialized with a size_t array of the given size, if successful; otherwise, a NULL
 * pointer.
 */
struct labels_t * initLabels(size_t lblcount) {
    size_t *larr = NULL;
    struct labels_t *lbl = NULL;
    larr = (size_t *)malloc(sizeof(size_t *) * lblcount);
    if (larr != NULL) {
        lbl = (struct labels_t *)malloc(sizeof(struct labels_t));
        if (lbl != NULL) {
            lbl->labelcount = lblcount;
            lbl->labelarr = larr;
        } else {
            //malloc didn't work--free up
            free(larr);
        }
    }
    return lbl;
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
 * @brief Allocate and initialize an edge
 * @return pointer to new edge_t memory, if successful; otherwise NULL.
 */
struct edge_t * initEdge() {
    struct edge_t *edge;
    edge = (struct edge_t *)malloc(sizeof(struct edge_t));
    if (edge != NULL) {
        edge->attrs = NULL;
        edge->next = NULL;
        edge->prev = NULL;
        edge->u = 0;
        edge->v = 0;
        edge->cap = 0.0;
        edge->flow = 0.0;
    }
}


/**
 * @brief Clear the graph and all underlying structures
 *
 * The pointer itself will be changed to NULL
 *
 * @param gptr pointer-to-pointer for graph_t structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyGraph(void** gptr) {
    return 0;
}

/**
 * @brief Clear out the dimensions and all underlying structures
 * @param dptr pointer-to-pointer for dimensions_t structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyDimensions(void** dptr) {
    int retval = 0;
    if (NULL != *dptr) {
        struct dimensions_t *dims = *dptr;
        size_t *dimarr = dims->dimarr;
        if (NULL != dimarr) {
            free(dimarr);
            dims->dimarr = NULL;
        }
        dims->dimcount = 0;
        free(*dptr);
        *dptr = NULL;
        retval = 1;
    }
    return retval;
}

/**
 * @brief Clear out the label and all underlying structures
 * @param labels pointer-to-pointer for labels_t structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyLabels(void** lptr) {
    int retval = 0;
    if (NULL != *lptr) {
        struct labels_t *labels = *lptr;
        size_t *larr = labels->labelarr;
        if (NULL != larr) {
            free(larr);
            labels->labelarr = NULL;
        }
        labels->labelcount = 0;
        free(*lptr);
        *lptr = NULL;
        retval = 1;
    }
    return retval;
}

/**
 * @brief Clear an edge structure and any linked edges (use on single or a path)
 *
 * The pointer itself will be changed to NULL
 *
 * @param eptr pointer-to-pointer for initial edge pointer
 * @return 1 if success; 0 if error.
 */
int destroyEdges(void** eptr) {
    return 0;
}

/**
 * @brief Clear a graphops_t structure.
 *
 * The graph itself will not be cleared, only the reference to it.  The pointer itself will be changed to NULL
 *
 * @param opsptr pointer-to-pointer for graphops_t structuure to be cleared and deallocated
 * @return 1 if successful; 0 if error
 */
int destroyGraphops(void** opsptr) {
    int retval = 0;
    if (NULL != *opsptr) {
        struct graphops_t *gops = *opsptr;
        gops->g = NULL;
        gops->setCapacity = NULL;
        gops->resetGraph = NULL;
        gops->nodePath = NULL;
        gops->nodeCount = NULL;
        gops->getNode = NULL;
        gops->getNeighbors = NULL;
        gops->getEdges = NULL;
        gops->getEdge = NULL;
        gops->edgePath = NULL;
        gops->edgeCount = NULL;
        gops->addNode = NULL;
        gops->addEdge = NULL;
        free(*opsptr);
        *opsptr = NULL;
        retval = 1;
    }
    return retval;
}

/**
 * @brief Clear a node or node list (use on single or a path)
 *
 * The pointer itself will be changed to NULL
 *
 * @param nptr pointer-to-pointer for initial node structure
 * @return 1 if successful; 0 if error
 */
int destroyNodes(void** nptr) {
    return 0;
}

/**
 * @brief Clear a feature or feature list (use on single or multiple attributes)
 *
 * The pointer itself will be changed to NULL
 *
 * @param fptr pointer-to-pointer for initial feature structure
 * @return 1 if successful; 0 if error
 */
int destroyFeatures(void** fptr) {
    return 0;
}