//
// Created by david on 11/11/17.
//

#include<graphInit.h>

#include <util/crudops.h>
#include <impl/array/arraygraph.h>
#include <impl/array/arrayops.h>
#include <impl/link/linkgraph.h>
#include <impl/link/linkops.h>
#include <impl/memmap/mmapgraph.h>
#include <impl/memmap//mmapops.h>

const short OP_SUCCESS = 0;
const short OP_FAIL = -1;

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
}

static void setMMapOps(struct graphops_t *gops) {
    //Node operations
    gops->addNode = mmapAddNode;
    gops->getNode = mmapGetNode;
    gops->nodeCount = mmapNodeCount;
    gops->getNeighbors = mmapGetNeighbors;
    gops->removeNode = mmapRemoveNode;

    //Edge operations
    gops->addEdge = mmapAddEdge;
    gops->getEdge = mmapGetEdge;
    gops->getEdges = mmapGetEdges;
    gops->removeEdge = mmapRemoveEdge;
    gops->edgeCount = mmapEdgeCount;

    //Value operations
    gops->setCapacity = mmapSetCapacity;
    gops->addCapacity = mmapAddCapacity;
    gops->getCapacity = mmapGetCapacity;
    gops->setFlow = mmapSetFlow;
    gops->addFlow = mmapAddFlow;
    gops->getFlow = mmapGetFlow;

    //Reset operations
    gops->resetGraph = mmapResetGraph;

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

    struct graph_t *graph = NULL;

    //Create switch selectors for graph types
    enum GRAPHDOMAIN dirtype;
    enum GRAPHDOMAIN imptype;
    enum GRAPHDOMAIN labtype;
    enum GRAPHDOMAIN domaintype;

    if (parseTypeFlags(&typeflags, &dirtype, &imptype, &labtype, &domaintype)) {
        //need dimensions for array type
        //TODO:  Better or more general way to handle ARRAY?
        if (imptype == ARRAY && dims == NULL) {
            return NULL;
        }

        //need label size for LABELED
        if (lblcount == 0 && labtype == LABELED) {
            return NULL;
        }

        graph = basicGraphInit();
        if (graph != NULL) {
            struct labels_t *labels = NULL;
            if (labtype == LABELED) {
                labels = initLabels(lblcount);
            }

            graph->gtype = typeflags;
            graph->dims = dims;
            graph->labels = labels;
            int initSuccess = 0;
            switch(imptype) {
                case ARRAY:
                    initSuccess = arrayGraphInit(graph);
                    break;
                case HASHED:
                    //initSuccess = hashGraphInit(g);
                    break;
                default:
                    initSuccess = linkGraphInit(graph);
                    break;
            }
            if (!initSuccess) {
                //something went wrong--clean up
                clearGraph(graph);
                destroyGraph((void **)&(graph));
            }
        }

    }

    return graph;
}

/**
 * @brief Stand up a shared graph according to the settings. This may be as a memory-only or file-backed structure.
 * 
 * @param typeFlags Type of graph to create
 * @param shareFlags Sharing/access settings
 * @param lblCount Number of labels to be used
 * @param dims Dimensional characteristics
 * @param sharedMeta Shared metaadata structure
 * @return reference to a fully initialized graph structure
 */
struct graph_t * initSharedGraph(enum GRAPHDOMAIN typeFlags, enum GRAPHACCESS shareFlags, size_t lblCount, struct dimensions_t *dims, void *sharedMeta) {
    struct graph_t *graph = NULL;

    //Create switch selectors for graph types
    enum GRAPHDOMAIN dirtype;
    enum GRAPHDOMAIN imptype;
    enum GRAPHDOMAIN labtype;
    enum GRAPHDOMAIN domaintype;

    enum GRAPHACCESS sharedFlag;
    enum GRAPHACCESS savedFlag;
    enum GRAPHACCESS fileFlag;
    enum GRAPHACCESS rwFlag;

    if (parseTypeFlags(&typeFlags, &dirtype, &imptype, &labtype, &domaintype) == OP_SUCCESS
        && parseAccessFlags(&shareFlags, &sharedFlag, &savedFlag, &fileFlag, &rwFlag) == OP_SUCCESS) {
        //need dimensions for array type
        //TODO:  Better or more general way to handle ARRAY?
        if (dims == NULL) {
            return NULL;
        }

        //need label size for LABELED
        if (lblCount == 0 && labtype == LABELED) {
            return NULL;
        }

        graph = basicGraphInit();
        if (graph != NULL) {
            struct labels_t *labels = NULL;
            if (labtype == LABELED) {
                labels = initLabels(lblCount);
            }

            graph->gtype = typeFlags;
            graph->dims = dims;
            graph->labels = labels;
            int initSuccess = 0;
            switch(imptype) {
                case ARRAY:
                    initSuccess = arrayGraphInit(graph);
                    break;
                case HASHED:
                    //initSuccess = hashGraphInit(g);
                    break;
                default:
                    initSuccess = linkGraphInit(graph);
                    break;
            }
            if (!initSuccess) {
                //something went wrong--clean up
                clearGraph(graph);
                destroyGraph((void **)&(graph));
            }
        }

    }

    return graph;
    
}


/**
 * @brief Create and fill the graphops_t structure that handles basic operations for the graph
 *
 * Using the graph_t implementation pass, this function creates a graphops_t structure necessary to handle basic functions
 * for the given graph.
 *
 * @param graph Graph structure being used
 * @return Pointer to a new graphops_t structure, if successful; otherwise, a pointer to NULL
 *
 */
struct graphops_t * getOperations(struct graph_t *graph) {
    struct graphops_t *gops = NULL;
    //Create switch selectors for graph types

    if (graph != NULL) {
        enum GRAPHDOMAIN dirtype;
        enum GRAPHDOMAIN imptype;
        enum GRAPHDOMAIN labtype;
        enum GRAPHDOMAIN domaintype;
        enum GRAPHDOMAIN gflags = graph->gtype;
        if (parseTypeFlags(&gflags, &dirtype, &imptype, &labtype, &domaintype)) {
            gops = initGraphops();
            gops->graph = graph;
            switch (imptype) {
                case ARRAY:
                    setArrayOps(gops);
                    break;
                case LINKED:
                    setLinkOps(gops);
                    break;
                default:
                    //TODO:  Do the other implementations
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
 * @param graph Graph to be cleared
 * @return 1 if successful; otherwise, 0.
 */
short clearGraph(struct graph_t *graph) {
    short retval = OP_SUCCESS;
    if (graph != NULL) {
        enum GRAPHDOMAIN dirtype;
        enum GRAPHDOMAIN imptype;
        enum GRAPHDOMAIN labtype;
        enum GRAPHDOMAIN domaintype;
        enum GRAPHDOMAIN gflags = graph->gtype;
        if (parseTypeFlags(&gflags, &dirtype, &imptype, &labtype, &domaintype)) {

            switch (imptype) {
                case ARRAY:
                    retval = retval & arrayGraphFree(graph);
                    break;
                case LINKED:
                    retval = retval & linkGraphFree(graph);
                    break;
                case HASHED:
                    //TODO:  implement clearing operations
                    break;
                default:
                    break;
            }
        }
    }
    return retval;
}

