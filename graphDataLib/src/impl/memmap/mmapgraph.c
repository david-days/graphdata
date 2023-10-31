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

struct mmapdata_t * initMMapMeta(void *addr, int prot_flags, int mmap_flags, int metaFd) {
    struct mmapdata_t *meta = (struct mmapdata_t *)mmap(addr, sizeof(struct mmapdata_t), prot_flags, mmap_flags, metaFd, 0);
    return meta;
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
