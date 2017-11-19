//
// Created by david on 11/18/17.
//

#include <impl/arraygraph.h>
#include <util/spatial.h>
#include <stdlib.h>
#include <impl/arrayops.h>

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
        ameta->conncount = 0;
    }
    return ameta;
}

/**
 * @brief Utiltiy function to free up allocated memory for array-graph metadata
 * @param metaptr pointer-to-pointer for metadata
 * @return 1 if successful; 0 if error.
 */
static int freeArrayMeta(void** metaptr) {
    int retval = 0;
    if (*metaptr != NULL) {
        struct arraydata_t *mptr = (struct arraydata_t *)metaptr;
        mptr->conncount = 0;
        mptr->edgelen = 0;
        mptr->nodelen = 0;
        free(*metaptr);
        *metaptr = NULL;
        retval = 1;
    }
    return retval;
}

/**
 * @brief Utility method to create a size_t **array, returned as a void *
 * @param alen Length of base array (nodes)
 * @param conlen Connectivity count (how many neighbors, or dimensionality of the array)
 * @return size_t **array as a void *.
 */
static void * createNodeArray(size_t alen, size_t conlen) {
    void *arrbase = malloc(sizeof(size_t *)*alen);
    if (arrbase != NULL) {
        size_t **arrvals = (size_t **) arrbase;
        for (size_t i=0;i<alen;i++) {
            size_t *conarr = (size_t *)malloc(sizeof(size_t *)*conlen);
            if (conarr != NULL)
                arrvals[i] = conarr;
        }
    }
    return arrbase;
}

/**
 * @brief Utility function to create a double **array, returned as a void *
 * @param alen Length of base array (nodes)
 * @param conlen Connectivity count (how many neighbors, or dimensionality of the array)
 * @return double **array as a void *.
 */
static void * createDoubleArray(size_t alen, size_t conlen) {
    void *arrbase = malloc(sizeof(double *)*alen);
    if (arrbase != NULL) {
        double **arrvals = (double **) arrbase;
        for (size_t i=0;i<alen;i++) {
            double *caparr = (double *)malloc(sizeof(double *)*conlen);
            if (caparr != NULL)
                arrvals[i] = caparr;
        }
        //initialize to zeros
        for (size_t i=0;i<alen;i++) {
            for (size_t j=0;j<conlen;j++) {
                arrvals[i][j] = 0.0;
            }
        }
    }
    return arrbase;
}

/**
 * @brief Set up a graph with array backing data
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @return 1 if successful; 0 if an error
 */
int arrayGraphInit(enum GRAPHTYPE gtype, struct graph_t *g) {
    int retval = 0;
    if (NULL == g) return 0;
    //Can't continue if no dimensions
    if (g->dims == NULL) return 0;

    size_t arrlen = indexLength(g->dims);
    if (arrlen > 0) {
        struct arraydata_t *arrmeta = initArrayMeta();
        arrmeta->nodelen = arrlen;
        arrmeta->edgelen = arrlen;
        switch (gtype) {
            case UNDIRECTED:
                //undirected graphs use min-to-max pair connectivity
                arrmeta->conncount = g->dims->dimcount;
                break;
            default:
                //all other graphs (currently DIRECTED) use bidirectional connectivity within the graph
                arrmeta->conncount = 2 * g->dims->dimcount;
                break;
        }
        //Create the supporting arrays
        g->nodeImpl = createNodeArray(arrmeta->nodelen, arrmeta->conncount);
        g->edgeImpl = createDoubleArray(arrmeta->edgelen, arrmeta->conncount);
        g->capImpl = createDoubleArray(arrmeta->edgelen, arrmeta->conncount);
        g->flowImpl = createDoubleArray(arrmeta->edgelen, arrmeta->conncount);
        g->metaImpl = (void *)arrmeta;
        if (g->nodeImpl != NULL && g->edgeImpl != NULL
                && g->capImpl != NULL && g->flowImpl != NULL)
            retval = 1;
    }
    return retval;
}


/**
 * @brief Set up a graph with array and backing data, adding on label connections as necessary.
 * @param gtype DIRECTED or UNDIRECTED graph
 * @param g Graph structure
 * @param lblcount Number of label nodes to be added
 * @return 1 if successful; 0 of an error.
 */
int arrayGraphLabelInit(enum GRAPHTYPE gtype, struct graph_t *g, size_t lblcount) {
    int retval = 0;

    return retval;
}


/**
 * @brief Perform clearing operations to deallocate the array graph internal values and structures.
 * @param g graph_t with array structures to be deallocated
 * @return 1 if successful; otherwise, 0.
 */
int arrayGraphFree(struct graph_t *g) {
    int retval = 0;
    if (NULL != g) {
        //First, use the arrayMeta to clean up the graph arrays
        struct arraydata_t *arrmeta = (struct arraydata_t *)g->metaImpl;
        freeGraphArray(arrmeta->nodelen, &(g->nodeImpl));
        freeGraphArray(arrmeta->edgelen, &(g->edgeImpl));
        freeGraphArray(arrmeta->edgelen, &(g->flowImpl));
        freeGraphArray(arrmeta->edgelen, &(g->capImpl));
        //Lastly, free up the arraydata_t memory
        freeArrayMeta(&(g->metaImpl));
        retval = 1;
    }
    return retval;
}