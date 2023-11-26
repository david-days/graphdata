//
// Created by david on 10/21/23.
//

#include <impl/memmap/mmapgraph.h>
#include <util/cartesian.h>
#include <util/crudops.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

short rebuildExistingMMapGraph(struct graph_t *g, struct mmapdata_t *meta) {
    return OP_FAIL;
}

short startFreshMMapGraph(struct graph_t *g, struct mmapdata_t *gmeta) {
    short retVal = OP_FAIL;
    
    enum GRAPHDOMAIN dirtype;
    enum GRAPHDOMAIN imptype;
    enum GRAPHDOMAIN labtype;
    enum GRAPHDOMAIN domaintype;

    enum GRAPHACCESS sharedFlag;
    enum GRAPHACCESS savedFlag;
    enum GRAPHACCESS fileFlag;
    enum GRAPHACCESS rwFlag;

    if (parseTypeFlags(&g->gtype, &dirtype, &imptype, &labtype, &domaintype) == OP_SUCCESS
        && parseAccessFlags(&g->gaccess, &sharedFlag, &savedFlag, &fileFlag, &rwFlag) == OP_SUCCESS) {
        const size_t cartlen = cartesianIndexLength(g->dims);
        size_t nodeLen = cartlen;
        //if label graph, modify the array length accordingly
        if (labtype == LABELED) {
            nodeLen = g->labels->labelcount * cartlen;
        }

        if (nodeLen > 0) {
            gmeta->nodelen = nodeLen;
            gmeta->edgelen = nodeLen;
            //undirected graphs use min-to-max pair connectivity
            gmeta->degree = g->dims->dimcount;
            //Create the supporting arrays
            short writable = rwFlag == GRAPH_WRITE ? PROT_READ | PROT_WRITE : PROT_READ;
            short sharing = sharedFlag == PRIVATE ? MAP_PRIVATE : MAP_SHARED;
            int fdNode = -1;
            g->nodeImpl = initMmapInt(nodeLen, gmeta->degree, 1, writable, sharing, fdNode);
            //In this implementation, the node array also holds the edges, so we don't need the extra memory
            g->edgeImpl = NULL;
            g->capImpl = createDoubleArray(gmeta->edgelen, gmeta->degree);
            g->flowImpl = createDoubleArray(gmeta->edgelen, gmeta->degree);
            g->metaImpl = (void *) gmeta;
            if (g->nodeImpl != NULL && g->capImpl != NULL &&
                g->flowImpl != NULL)
                retVal = OP_SUCCESS;
        }

    }
    
    return retVal;
}

struct mmapdata_t * initMMapMeta(void *addr, int prot_flags, int mmap_flags, int metaFd) {
    struct mmapdata_t *meta = (struct mmapdata_t *)mmap(addr, sizeof(struct mmapdata_t), prot_flags, mmap_flags, metaFd, 0);
    return meta;
}

/**
 * @brief Set up a graph with memory-mapped files backing data
 *
 * @param g Graph structure
 * @param lblcount Number of labels to be used--may be zero, depending on the graph domain.
 * @return 1 if successful; 0 if an error
 */
short mmapGraphInit(struct graph_t *g) {
    short retval = OP_FAIL;
    if (NULL == g) return OP_FAIL;
    //Can't continue if no dimensions
    if (g->dims == NULL) return OP_FAIL;
    //Create switch selectors for graph types
    enum GRAPHDOMAIN dirtype;
    enum GRAPHDOMAIN imptype;
    enum GRAPHDOMAIN labtype;
    enum GRAPHDOMAIN domaintype;

    enum GRAPHACCESS sharedFlag;
    enum GRAPHACCESS savedFlag;
    enum GRAPHACCESS fileFlag;
    enum GRAPHACCESS rwFlag;
    
    //parse type flags, and quit if not parsable
    if (parseTypeFlags(&g->gtype, &dirtype, &imptype, &labtype, &domaintype) == OP_FAIL
    || parseAccessFlags(&g->gaccess, &sharedFlag, &savedFlag, &fileFlag, &rwFlag) == OP_FAIL) {
        return OP_FAIL;
    }

    struct mmapdata_t *gmeta = (struct mmapdata_t *)g->metaImpl;

    if (savedFlag != EXISTING) {
        retval = startFreshMMapGraph(g, gmeta);
    } else {
        retval = rebuildExistingMMapGraph(g, gmeta);
    }
    return retval;
}


/**
 * @brief Perform clearing operations to deallocate the memory-mapped addresses graph internal values and structures.
 * @param g graph_t with array structures to be deallocated
 * @return 1 if successful; otherwise, 0.
 */
short mmapGraphFree(struct graph_t *g) {
    return OP_FAIL;
}


void * initMmapInt(size_t nlen, size_t conlen, size_t zeroOut, int prot_flags, int mmap_flags, int nodeFd) {
    size_t fileLen = nlen * conlen;
    void *nodeBase = mmap(NULL, sizeof(size_t *) * fileLen, prot_flags, mmap_flags, nodeFd, 0);
    if (nodeBase != NULL && zeroOut > 0 && (prot_flags & PROT_WRITE) == PROT_WRITE)  {
        size_t *nodeVals = (size_t *)nodeBase;
        for (size_t *p = nodeVals; p < nodeVals+fileLen;p++) {
           *p = 0;
        }
    }
    return nodeBase;
}

void * initMMapDouble(size_t nlen, size_t conlen, size_t zeroOut, int prot_flags, int mmap_flags, int doubleFd) {
    size_t doubleLen = nlen * conlen;
    void *doubleBase = mmap(NULL, sizeof(double *)*doubleLen, prot_flags, mmap_flags, doubleFd, 0);
    if (doubleBase != NULL && zeroOut > 0 && (prot_flags & PROT_WRITE) == PROT_WRITE) {
        double *doubleVals = (double *) doubleBase;
        for (double *p = doubleVals; p < doubleVals+doubleLen;p++) {
            *p = 0.0;
        }
    }
    return doubleBase;
}

short freeMMapMeta(void **metaptr) {
    short retval = OP_FAIL;
    if (*metaptr != NULL) {
        munmap(metaptr, sizeof(struct mmapdata_t));
        retval = OP_SUCCESS;
    }
    return retval;
}

short freeMMapInt(void **nodeArr, size_t bucketCount) {
    short retVal = OP_FAIL;
    if (*nodeArr != NULL) {
        munmap(nodeArr, bucketCount);
        retVal = OP_SUCCESS;
    }
    return retVal;
}

short freeMMapDouble(void **dblArr, size_t bucketCount) {
    short retVal = OP_FAIL;
    if (*dblArr != NULL) {
        munmap(dblArr, bucketCount);
        retVal = OP_SUCCESS;
    }
    return retVal;
}
