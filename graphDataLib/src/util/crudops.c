//
// Created by david on 11/27/17.
//

#include <util/crudops.h>
#include <stdarg.h>



int parseTypeFlags(enum GRAPHDOMAIN *tflags, enum GRAPHDOMAIN *dirflag, enum GRAPHDOMAIN *impflag,
                   enum GRAPHDOMAIN *lblflag, enum GRAPHDOMAIN *domflag) {
    //retval used for future cases where illegitimate combinations may be used.
    int retval = 0;
    if (*tflags == DEFAULTSELECT) *tflags = DEFAULTGRAPH;
    //Create switch selectors for graph types
    enum GRAPHDOMAIN dirtype = (DIRSELECT) & *tflags;
    enum GRAPHDOMAIN imptype = (IMPLSELECT) & *tflags;
    enum GRAPHDOMAIN labtype = (LABELSELECT) & *tflags;
    enum GRAPHDOMAIN domaintype = (DOMAINSELECT) & *tflags;

    //check and set defaults for empty values
    if (dirtype == 0) dirtype = UNDIRECTED;
    if (imptype == 0) imptype = LINKED;
    if (labtype == 0) labtype = UNLABELED;
    if (domaintype == 0) domaintype = GENERIC;

    *dirflag = dirtype;
    *impflag = imptype;
    *lblflag = labtype;
    *domflag = domaintype;

    //write the cleaned-up values back to the reference
    *tflags = dirtype | imptype | labtype | domaintype;

    retval = 1;

    return retval;
}


//Create operations
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
        gops->graph = NULL;
        gops->addEdge = NULL;
        gops->addNode = NULL;
        gops->edgeCount = NULL;
        gops->getEdge = NULL;
        gops->getEdges = NULL;
        gops->getNeighbors = NULL;
        gops->getNode = NULL;
        gops->nodeCount = NULL;
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
    return edge;
}


/**
 * @brief Allocate and initialize a node
 *
 * @return pointer to new node_t memory, if successful; otherwise, NULL.
 */
struct node_t * initNode() {
    struct node_t *node = NULL;
    node = (struct node_t *)malloc(sizeof(struct node_t));
    if (node != NULL) {
        node->next = NULL;
        node->prev = NULL;
        node->nodeid = 0;
        node->attrs = NULL;
        node->edges = NULL;
    }
    return node;
}

/**
 * @brief Allocate and initialize a feature structure
 * @return pointer to new feature_t memory, if successful; otherwise, NULL.
 */
struct feature_t * initFeature() {
    struct feature_t *f = NULL;
    f = (struct feature_t *)malloc(sizeof(struct feature_t));
    if (f != NULL) {
        f->val = 0.0;
        f->hashid = 0;
        f->fdata = NULL;
        f->prev = NULL;
        f->next = NULL;
    }
    return f;
}



//Clone operations

/**
 * @brief Create a copy of the given node
 *
 * This operation is intended to be used for extracting data from the graph to be used for external purposes, such as
 * delineating paths, without unintentionally altering the underlying graph structure.
 *
 * @param onode Original node_t to be cloned
 * @return pointer to the cloned structure, if successful; otherwise, a pointer to NULL
 */
struct node_t * cloneNode(const struct node_t *onode) {
    struct node_t *nnode = NULL;
    if (onode != NULL) {
        nnode = initNode();
        if (nnode != NULL) {
            nnode->nodeid = onode->nodeid;
        }
    }
    return nnode;
}

/**
 * @brief Create a copy of the given edge
 *
 * This operation is intended to be used for extracting data from the graph to be used for external purposes, such as
 * processing stacks, queues, and paths, without unintentionally altering the underlying graph structure.
 *
 * @param oedge Original edge_t structure to be cloned.
 * @return pointer to the cloned data, if successful; otherwise, a pointer to NULL
 */
struct edge_t * cloneEdge(const struct edge_t *oedge) {
    struct edge_t *nedge = NULL;
    if (oedge != NULL) {
        nedge = initEdge();
        if (nedge != NULL) {
            nedge->u = oedge->u;
            nedge->v = oedge->v;
            nedge->cap = oedge->cap;
            nedge->flow = oedge->flow;
        }
    }
    return nedge;
}

/**
 * @brief Create a copy of the given feature data
 *
 * @param ofeat Original feature to be copied.
 * @return pointer to the cloned data, if successful; otherwise, a pointer to NULL
 */
struct feature_t * cloneFeature(const struct feature_t *ofeat) {
    struct feature_t *nf = NULL;
    nf = initFeature();
    if (nf != NULL) {
        //TODO:  Explicitly state fdata and featurename refer to originals, or work on making copies?
        nf->fdata = ofeat->fdata;
        nf->hashid = ofeat->hashid;
        nf->val = ofeat->val;
        nf->featurename = ofeat->featurename;
    }
    return nf;
}




//Destroy/clearing operations

/**
 * @brief Clear the graph and all underlying standard structures
 *
 * The pointer itself will be changed to NULL
 *
 * @param gptr pointer-to-pointer for graph_t structure to be cleared
 * @return 1 if success; 0 if error
 */
int destroyGraph(void** gptr) {
    int retval = 0;
    if (NULL != *gptr) {
        free(*gptr);
        *gptr = NULL;
        retval = 1;
    }
    return retval;
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
    int retval = 0;
    if (*eptr == NULL) return 1;
    struct edge_t *curr = (struct edge_t *)*eptr;
    struct edge_t *next;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    *eptr = NULL;
    retval = 1;

    return retval;
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
        gops->graph = NULL;
        gops->setCapacity = NULL;
        gops->resetGraph = NULL;
        gops->nodeCount = NULL;
        gops->getNode = NULL;
        gops->getNeighbors = NULL;
        gops->getEdges = NULL;
        gops->getEdge = NULL;
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
    int retval = 0;
    if (*nptr != NULL) {
        struct node_t *curr = (struct node_t *)*nptr;
        struct node_t *next;
        while (curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }
        *nptr = NULL;
        retval = 1;
    }

    return retval;
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
    int retval = 0;
    if (*fptr != NULL) {
        struct feature_t *curr = (struct feature_t *)*fptr;
        struct feature_t *next;
        while (curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }
        *fptr = NULL;
        retval = 1;
    }

    return retval;
}