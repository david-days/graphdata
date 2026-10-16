//
// Created by david on 10/21/23.
//


#include <impl/sharedmmapgraph.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <cartesian.h>
#include <crudops.h>
#include <sys/mman.h>

static struct shmmapdata_t * initShmmapMeta() {
    struct shmmapdata_t *mapmeta = NULL;
    mapmeta = (struct shmmapdata_t *) malloc(sizeof(struct shmmapdata_t));
    if (mapmeta != NULL) {
        mapmeta->nodelen = 0;
        mapmeta->edgelen = 0;
        mapmeta->degree = 0;
        mapmeta->arraylen = 0;
        mapmeta->roflag = 0;
        mapmeta->nodefile = NULL;
        mapmeta->capfile = NULL;
        mapmeta->flowfile = NULL;
    }
    return mapmeta;
}

static void clearMmapFile(char **pathptr) {
    if (pathptr != NULL && *pathptr != NULL) {
        unlink(*pathptr);
        free(*pathptr);
        *pathptr = NULL;
    }
}

static int freeShmmapMeta(struct shmmapdata_t *meta) {
    int retVal = EXIT_FAILURE;
    if (meta != NULL) {
        clearMmapFile(&meta->nodefile);
        clearMmapFile(&meta->capfile);
        clearMmapFile(&meta->flowfile);
        meta->nodelen = 0;
        meta->edgelen = 0;
        meta->degree = 0;
        meta->arraylen = 0;
        free(meta);
        retVal = EXIT_SUCCESS;
    }
    return retVal;
}

int shmmapGraphInit(struct graph_t *g) {
    int retVal = EXIT_FAILURE;
    if (NULL == g) return EXIT_FAILURE;
    if (g->dims == NULL) return EXIT_FAILURE;
    enum GRAPHDOMAIN dirtype, imptype, labtype, domaintype, rdtype;
    if (parseTypeFlags(&g->gtype, &dirtype, &imptype, &labtype, &domaintype, &rdtype) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    size_t cartlen = cartesianIndexLength(g->dims);
    size_t nlen = cartlen;
    if (labtype == LABELED) {
        nlen = g->labels->labelcount * cartlen;
    }
    int protflags = PROT_READ;
    int mapflags = MAP_SHARED;

    if (rdtype != RD_ONLY) {
        protflags |= PROT_WRITE;
    }

    if (nlen > 0) {
        struct shmmapdata_t *mapmeta = initShmmapMeta();
        if (mapmeta == NULL) {
            return EXIT_FAILURE;
        }
        mapmeta->roflag = rdtype;
        mapmeta->nodelen = nlen;
        mapmeta->edgelen = nlen;
        mapmeta->degree = g->dims->dimcount;
        mapmeta->arraylen = nlen * mapmeta->degree;
        g->metaImpl = (void *)mapmeta;
        g->nodeImpl = createNodeMmap(mapmeta, rdtype, protflags, mapflags);
        g->capImpl = createCapMmap(mapmeta, rdtype, protflags, mapflags);
        g->flowImpl = createFlowMmap(mapmeta, rdtype, protflags, mapflags);
        g->edgeImpl = NULL;
        if (g->nodeImpl != NULL && g->capImpl != NULL && g->flowImpl != NULL) {
            retVal = EXIT_SUCCESS;
        }
    }
    return retVal;

}

int shmmapGraphFree(struct graph_t *g) {
    int retVal = EXIT_SUCCESS;
    if (NULL != g && g->metaImpl != NULL) {
        struct shmmapdata_t *mapmeta = (struct shmmapdata_t *) g->metaImpl;
        retVal = retVal | freeMapMem(g->nodeImpl, mapmeta->arraylen * sizeof(size_t));
        g->nodeImpl = NULL;
        retVal = retVal | freeMapMem(g->capImpl, mapmeta->arraylen * sizeof(double));
        g->capImpl = NULL;
        retVal = retVal | freeMapMem(g->flowImpl, mapmeta->arraylen * sizeof(double));
        g->flowImpl = NULL;
        retVal = retVal | freeShmmapMeta(mapmeta);
        g->metaImpl = NULL;
    }
    return retVal;
}
