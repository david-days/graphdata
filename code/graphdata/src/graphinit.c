//
// Created by david on 11/11/17.
//

#include<graphinit.h>

#include <stdlib.h>
#include <util/crudops.h>
#include <impl/arraygraph.h>
#include <impl/arrayops.h>
#include <impl/linkgraph.h>
#include <impl/linkops.h>
#include <impl/hashgraph.h>


static void setArrayOps(struct graphops_t *gops) {
    //Node operations
    gops->addNode = arrayAddNode;
    gops->getNode = arrayGetNode;
    gops->nodeCount = arrayNodeCount;
    gops->nodePath = arrayGetNodePath;
    gops->getNeighbors = arrayGetNeighbors;
    gops->removeNode = arrayRemoveNode;

    //Edge operations
    gops->addEdge = arrayAddEdge;
    gops->getEdge = arrayGetEdge;
    gops->getEdges = arrayGetEdges;
    gops->removeEdge = arrayRemoveEdge;
    gops->edgeCount = arrayEdgeCount;
    gops->edgePath = arrayGetEdgePath;

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
    gops->nodePath = linkGetNodePath;
    gops->getNeighbors = linkGetNeighbors;
    gops->removeNode = linkRemoveNode;

    //Edge operations
    gops->addEdge = linkAddEdge;
    gops->getEdge = linkGetEdge;
    gops->getEdges = linkGetEdges;
    gops->removeEdge = linkRemoveEdge;
    gops->edgeCount = linkEdgeCount;
    gops->edgePath = linkGetEdgePath;

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
    if (g != NULL) {
        g->gimpl = impltype;
        g->dims = dims;
        switch(impltype) {
            case ARRAY:
                arrayGraphInit(gtype, g);
                break;
            case HASHED:
                hashGraphInit(gtype, g);
                break;
            default:
                linkGraphInit(gtype, g);
                break;
        }
    }
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
    if (g != NULL) {
        g->gimpl = impltype;
        g->dims = dims;
        switch(impltype) {
            case ARRAY:
                arrayGraphLabelInit(gtype, g, lblcount);
                break;
            case HASHED:
                hashGraphLabelInit(gtype, g, lblcount);
                break;
            default:
                linkGraphLabelInit(gtype, g, lblcount);
                break;
        }
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
    if (g != NULL) {
        gops = initGraphops();
        gops->g = g;
        switch (g->gimpl) {
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
    return gops;
}


