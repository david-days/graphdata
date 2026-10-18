//
// Created by david on 11/11/17.
//

#include<graphInit.h>

#include <stdlib.h>
#include <util/crudops.h>
#include <impl/arraygraph.h>
#include <impl/arrayops.h>
#include <impl/linkgraph.h>
#include <impl/linkops.h>
#include <impl/hashgraph.h>
#include <impl/hashops.h>
#include <impl/sharedmemgraph.h>
#include <impl/sharedmemops.h>
#include <impl/sharedmmapgraph.h>
#include <impl/sharedmmapops.h>



static void setArrayOps(struct graphops_t *gops) {
    //Node operations
    gops->addNode = arrayAddNode;
    gops->getNode = arrayGetNode;
    gops->nodeCount = arrayNodeCount;
    gops->getNeighbors = arrayGetNeighbors;
    gops->removeNode = arrayRemoveNode;

    //Edge operations
    gops->addEdge = arrayAddEdge;
    gops->getEdge = arrayGetEdge;
    gops->getEdges = arrayGetEdges;
    gops->removeEdge = arrayRemoveEdge;
    gops->edgeCount = arrayEdgeCount;

    //Value operations
    gops->setCapacity = arraySetCapacity;
    gops->addCapacity = arrayAddCapacity;
    gops->getCapacity = arrayGetCapacity;
    gops->setFlow = arraySetFlow;
    gops->addFlow = arrayAddFlow;
    gops->getFlow = arrayGetFlow;

    //Reset operations
    gops->resetGraph = arrayResetGraph;

    gops->listNodes = arrayListNodes;
    gops->foreachNode = arrayForeachNode;
}

static void setLinkOps(struct graphops_t *gops) {
    //Node operations
    gops->addNode = linkAddNode;
    gops->getNode = linkGetNode;
    gops->nodeCount = linkNodeCount;
    gops->getNeighbors = linkGetNeighbors;
    gops->removeNode = linkRemoveNode;

    //Edge operations
    gops->addEdge = linkAddEdge;
    gops->getEdge = linkGetEdge;
    gops->getEdges = linkGetEdges;
    gops->removeEdge = linkRemoveEdge;
    gops->edgeCount = linkEdgeCount;

    //Value operations
    gops->setCapacity = linkSetCapacity;
    gops->addCapacity = linkAddCapacity;
    gops->getCapacity = linkGetCapacity;
    gops->setFlow = linkSetFlow;
    gops->addFlow = linkAddFlow;
    gops->getFlow = linkGetFlow;

    //Reset operations
    gops->resetGraph = linkResetGraph;

    gops->listNodes = linkListNodes;
    gops->foreachNode = linkForeachNode;
}

static void setHashOps(struct graphops_t *gops) {
    // Node ops implemented for this slice; edge/value ops stub to GRAPH_ERR/NULL
    gops->addNode = hashAddNode;
    gops->getNode = hashGetNode;
    gops->nodeCount = hashNodeCount;
    gops->getNeighbors = hashGetNeighbors;
    gops->removeNode = hashRemoveNode;

    gops->addEdge = hashAddEdge;
    gops->getEdge = hashGetEdge;
    gops->getEdges = hashGetEdges;
    gops->removeEdge = hashRemoveEdge;
    gops->edgeCount = hashEdgeCount;

    gops->setCapacity = hashSetCapacity;
    gops->addCapacity = hashAddCapacity;
    gops->getCapacity = hashGetCapacity;
    gops->setFlow = hashSetFlow;
    gops->addFlow = hashAddFlow;
    gops->getFlow = hashGetFlow;

    gops->resetGraph = hashResetGraph;

    gops->listNodes = hashListNodes;
    gops->foreachNode = hashForeachNode;
}

static void setShmemOps(struct graphops_t *gops) {
    //Node operations
    gops->addNode = shmemAddNode;
    gops->getNode = shmemGetNode;
    gops->nodeCount = shmemNodeCount;
    gops->getNeighbors = shmemGetNeighbors;
    gops->removeNode = shmemRemoveNode;

    //Edge operations
    gops->addEdge = shmemAddEdge;
    gops->getEdge = shmemGetEdge;
    gops->getEdges = shmemGetEdges;
    gops->removeEdge = shmemRemoveEdge;
    gops->edgeCount = shmemEdgeCount;

    //Value operations
    gops->setCapacity = shmemSetCapacity;
    gops->addCapacity = shmemAddCapacity;
    gops->getCapacity = shmemGetCapacity;
    gops->setFlow = shmemSetFlow;
    gops->addFlow = shmemAddFlow;
    gops->getFlow = shmemGetFlow;

    //Reset operations
    gops->resetGraph = shmemResetGraph;

    gops->listNodes = shmemListNodes;
    gops->foreachNode = shmemForeachNode;
}

static void setMmapOps(struct graphops_t *gops) {
    //Node operations
    gops->addNode = shmapAddNode;
    gops->getNode = shmapGetNode;
    gops->nodeCount = shmapNodeCount;
    gops->getNeighbors = shmapGetNeighbors;
    gops->removeNode = shmapRemoveNode;

    //Edge operations
    gops->addEdge = shmapAddEdge;
    gops->getEdge = shmapGetEdge;
    gops->getEdges = shmapGetEdges;
    gops->removeEdge = shmapRemoveEdge;
    gops->edgeCount = shmapEdgeCount;

    //Value operations
    gops->setCapacity = shmapSetCapacity;
    gops->addCapacity = shmapAddCapacity;
    gops->getCapacity = shmapGetCapacity;
    gops->setFlow = shmapSetFlow;
    gops->addFlow = shmapAddFlow;
    gops->getFlow = shmapGetFlow;

    //Reset operations
    gops->resetGraph = shmapResetGraph;

    gops->listNodes = shmapListNodes;
    gops->foreachNode = shmapForeachNode;
}

/**
 * @brief Initialize a graph according to the flags set in the GRAPHDOMAIN value.
 *
 * Using the flags set in the typeflags parameter, create a graph structure capable of supporting the requirements in an
 * efficient (memory or Big-O) manner.
 *
 * The parameter gtype holds the flags for what graph is desired.  For example, For a directed graph, used to represent a set of cartesian
 * coordinates and no extra label nodes, pass in the value created by DIRECTED | LINKED | SPATIAL | UNLABELED.
 *
 * If an empty set of flags are passed to the typeflags parameter, the returned graph will be UNDIRECTED | LINKED | GENERIC | UNLABELED.
 *
 * @param gtype Type of graph implementation to be created. Flag values set underlying structures and metadata.
 * @param lblcount Number of label nodes to be used within the graph.  Required for LABELED flag; ignored for all others.
 * @param dims Dimensional parameters structure.  Required for ARRAY | SPATIAL graphs; otherwise, may be NULL.  The returned graph will
 * hold the reference to the structure that was passed.
 * @return If successful and valied, initialized graph structure, according to the flags.  Otherwise, a NULL pointer.
 */
struct graph_t * initGraph(enum GRAPHDOMAIN typeflags, size_t lblcount, struct dimensions_t *dims) {

    struct graph_t *g = NULL;

    //Create switch selectors for graph types
    enum GRAPHDOMAIN dirtype, imptype, labtype, domaintype, rdtype;

    if (parseTypeFlags(&typeflags, &dirtype, &imptype, &labtype, &domaintype, &rdtype) == GRAPH_OK) {
        // Dimensional backends need an explicit dimensions structure
        if ((imptype == ARRAY || imptype == SHARED_MEM || imptype == SHARED_MMAP) && dims == NULL) {
            return NULL;
        }

        // RD_ONLY is only meaningful for shared backends
        if (rdtype == RD_ONLY && imptype != SHARED_MEM && imptype != SHARED_MMAP) {
            return NULL;
        }

        // need label size for LABELED
        if (lblcount == 0 && labtype == LABELED) {
            return NULL;
        }

        g = basicGraphInit();
        if (g != NULL) {
            struct labels_t *labels = NULL;
            if (labtype == LABELED) {
                labels = initLabels(lblcount);
            }

            g->gtype = typeflags;
            g->dims = dims;
            g->labels = labels;
            int initSuccess = EXIT_FAILURE;
            switch(imptype) {
                case ARRAY:
                    initSuccess = arrayGraphInit(g);
                    break;
                case HASHED:
                    initSuccess = hashGraphInit(g);
                    break;
                case SHARED_MEM:
                    initSuccess = shmemGraphInit(g);
                    break;
                case SHARED_MMAP:
                    initSuccess = shmmapGraphInit(g);
                    break;
                default:
                    initSuccess = linkGraphInit(g);
                    break;
            }
            if (initSuccess != EXIT_SUCCESS) {
                //something went wrong--clean up
                clearGraph(g);
                destroyGraph((void **)&(g));
            }
        }

    }

    return g;
}

struct graph_t * attachReadonlyGraph(struct graph_t *src) {
    if (src == NULL || src->dims == NULL) {
        return NULL;
    }

    enum GRAPHDOMAIN dirtype, imptype, labtype, domaintype, rdtype;
    enum GRAPHDOMAIN gflags = src->gtype;
    if (parseTypeFlags(&gflags, &dirtype, &imptype, &labtype, &domaintype, &rdtype) != GRAPH_OK) {
        return NULL;
    }
    if (imptype != SHARED_MEM && imptype != SHARED_MMAP) {
        return NULL;
    }
    if (rdtype == RD_ONLY) {
        return NULL;
    }

    struct graph_t *g = basicGraphInit();
    if (g == NULL) {
        return NULL;
    }

    g->dims = src->dims;
    g->labels = src->labels;
    g->gtype = (src->gtype | RD_ONLY);

    int ok = EXIT_FAILURE;
    if (imptype == SHARED_MEM) {
        ok = shmemGraphAttachReadonly(g, src);
    } else {
        ok = shmmapGraphAttachReadonly(g, src);
    }

    if (ok != EXIT_SUCCESS) {
        clearGraph(g);
        destroyGraph((void **)&g);
        return NULL;
    }
    return g;
}


/**
 * @brief Create and fill the graphops_t structure that handles basic operations for the graph
 *
 * Using the graph_t implementation pass, this function creates a graphops_t structure necessary to handle basic functions
 * for the given graph.
 *
 * @param g Graph structure being used
 * @return Pointer to a new graphops_t structure, if successful; otherwise, a pointer to NULL
 *
 */
struct graphops_t * getOperations(struct graph_t *g) {
    struct graphops_t *gops = NULL;
    //Create switch selectors for graph types

    if (g != NULL) {
        enum GRAPHDOMAIN dirtype, imptype, labtype, domaintype, rdtype;
        enum GRAPHDOMAIN gflags = g->gtype;
        if (parseTypeFlags(&gflags, &dirtype, &imptype, &labtype, &domaintype, &rdtype) == GRAPH_OK) {
            gops = initGraphops();
            gops->g = g;
            switch (imptype) {
                case ARRAY:
                    setArrayOps(gops);
                    break;
                case LINKED:
                    setLinkOps(gops);
                    break;
                case HASHED:
                    setHashOps(gops);
                    break;
                case SHARED_MEM:
                    setShmemOps(gops);
                    break;
                case SHARED_MMAP:
                    setMmapOps(gops);
                    break;
                default:
                    break;
            }
        }
    }
    return gops;
}


/**
 * @brief Clear out the graph's underlying structures, and null out the memory
 *
 * All underlying graph structures will be cleared and the associated memory to the structures freed.
 *
 * @param g Graph to be cleared
 * @return EXIT_SUCCESS if successful; otherwise, EXIT_FAILURE.
 */
int clearGraph(struct graph_t *g) {
    int retval = EXIT_SUCCESS;
    if (g != NULL) {
        enum GRAPHDOMAIN dirtype, imptype, labtype, domaintype, rdtype;
        enum GRAPHDOMAIN gflags = g->gtype;
        if (parseTypeFlags(&gflags, &dirtype, &imptype, &labtype, &domaintype, &rdtype) == GRAPH_OK) {

            switch (imptype) {
                case ARRAY:
                    retval = retval | arrayGraphFree(g);
                    break;
                case LINKED:
                    retval = retval | linkGraphFree(g);
                    break;
                case HASHED:
                    retval = retval | hashGraphFree(g);
                    break;
                case SHARED_MEM:
                    retval = retval | shmemGraphFree(g);
                    break;
                case SHARED_MMAP:
                    retval = retval | shmmapGraphFree(g);
                    break;
                default:
                    break;
            }
        }
    }
    return retval;
}

