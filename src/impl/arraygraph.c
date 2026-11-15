/**
 * This is the implementation of the array-based graph structure.  The primary purpose is speed and fixed memory usage,
 * in order to create predictable memory performance.
 *
 * The arrays are one-dimensional arrays of size nodecount x connectivity. Pointer math is used where possible
 * to allow quick access.
 *
 * The node structure also holds the edge structure, so graph_t->edgeImpl is NULL.
 */

#include <impl/arraygraph.h>
#include <util/cartesian.h>
#include <stdlib.h>
#include <impl/arrayops.h>
#include <util/crudops.h>

/**
 * @brief Utility function to create array-graph metatdata
 * @return Pointer to new metadata structure, if successful; NULL pointer, otherwise
 */
static struct arraydata_t * initArrayMeta() {
    struct arraydata_t *ameta = NULL;
    ameta = (struct arraydata_t *)malloc(sizeof(struct arraydata_t));
    if (ameta != NULL) {
        ameta->nodelen = 0;
        ameta->edgelen = 0;
        ameta->degree = 0;
        ameta->arraylen = 0;
    }
    return ameta;
}

/**
 * @brief Utilty function to free up allocated memory for array-graph metadata
 * @param metaptr pointer-to-pointer for metadata
 * @return GRAPH_OK if successful; GRAPH_ERR if error.
 */
static int freeArrayMeta(void** metaptr) {
    int retval = GRAPH_ERR;
    if (*metaptr != NULL) {
        struct arraydata_t *mptr = (struct arraydata_t *)metaptr;
        mptr->degree = 0;
        mptr->edgelen = 0;
        mptr->nodelen = 0;
        mptr->arraylen = 0;
        free(*metaptr);
        *metaptr = NULL;
        retval = GRAPH_OK;
    }
    return retval;
}

/**
 * @brief Utility method to create a size_t **array, returned as a void *
 * @param alen Length of base array (nodes)
 * @param conlen Connectivity count (how many neighbors, or dimensionality of the array)
 * @return size_t **array as a void *.
 */
static void * createNodeArray(size_t arraylen) {
    void *arrbase = malloc(sizeof(size_t) * arraylen);
    if (arrbase != NULL) {
        size_t *arrvals = (size_t *) arrbase;
        for (size_t *p = arrvals; p < arrvals + arraylen; p++) {
            *p = GRAPH_SLOT_EMPTY;
        }
    }
    return arrbase;
}

/**
 * @brief Utility function to create a double **array, returned as a void *
 *
 * Returns a double **array initialized to zeroes, returned as a void *
 * @param alen Length of base array (nodes)
 * @param conlen Connectivity count (how many neighbors, or dimensionality of the array)
 * @return double **array as a void *.
 *
 */
static void * createDoubleArray(size_t arraylen) {
    void *arrbase = malloc(sizeof(double) * arraylen);
    if (arrbase != NULL) {
        double *arrvals = (double *) arrbase;
        for (double *p = arrvals; p < arrvals + arraylen; p++) {
            *p = 0.0;
        }
     }
    return arrbase;
}

/**
 * @brief Set up a graph with array backing data
 *
 * @param g Graph structure. Requires a non-NULL dims.
 * @return GRAPH_OK when the arrays are allocated; GRAPH_ERR otherwise
 */
int arrayGraphInit(struct graph_t *g) {
    int retval = GRAPH_ERR;
    if (NULL == g) return GRAPH_ERR;
    //Can't continue if no dimensions
    if (g->dims == NULL) return GRAPH_ERR;
    //Create switch selectors for graph types
    enum GRAPHDOMAIN dirtype, imptype, labtype, domaintype, rdtype;
    //parse type flags, and quit if not parsable
    if (parseTypeFlags(&g->gtype, &dirtype, &imptype, &labtype, &domaintype, &rdtype) == GRAPH_ERR) {
        return GRAPH_ERR;
    }
    size_t cartlen = cartesianIndexLength(g->dims);
    size_t arrlen = cartlen;
    //if label graph, modify the array length accordingly
    if (labtype == LABELED) {
        arrlen = g->labels->labelcount * cartlen;
    }

    if (arrlen > 0) {
        struct arraydata_t *arrmeta = initArrayMeta();
        arrmeta->nodelen = arrlen;
        arrmeta->edgelen = arrlen;
        arrmeta->arraylen = arrlen * g->dims->dimcount;
        /* Degree is the dimension count for both directed and undirected graphs. */
        arrmeta->degree = g->dims->dimcount;
        //Create the supporting arrays
        g->nodeImpl = createNodeArray(arrmeta->arraylen);
        //In this implementation, the node array also holds the edges, so we don't need the extra memory
        g->edgeImpl = NULL;
        g->capImpl = createDoubleArray(arrmeta->arraylen);
        g->flowImpl = createDoubleArray(arrmeta->arraylen);
        g->metaImpl = (void *)arrmeta;
        if (g->nodeImpl != NULL && g->capImpl != NULL && g->flowImpl != NULL)
            retval = GRAPH_OK;
    }
    return retval;
}



/**
 * @brief Perform clearing operations to deallocate the array graph internal values and structures.
 * @param g graph_t with array structures to be deallocated
 * @return GRAPH_OK if successful; otherwise, GRAPH_ERR.
 */
int arrayGraphFree(struct graph_t *g) {
    int retval = GRAPH_ERR;
    if (NULL != g) {
        //First, use the arrayMeta to clean up the graph arrays
        struct arraydata_t *arrmeta = (struct arraydata_t *)g->metaImpl;
        freeGraphArray(arrmeta->nodelen, &(g->nodeImpl));
        freeGraphArray(arrmeta->edgelen, &(g->flowImpl));
        freeGraphArray(arrmeta->edgelen, &(g->capImpl));
        //Lastly, free up the arraydata_t memory
        freeArrayMeta(&(g->metaImpl));
        retval = GRAPH_OK;
    }
    return retval;
}