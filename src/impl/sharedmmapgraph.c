//
// Created by david on 10/21/23.
//


#include <impl/sharedmmapgraph.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
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

static void clearMmapFile(char **pathptr, int unlink_file) {
    if (pathptr != NULL && *pathptr != NULL) {
        if (unlink_file) {
            unlink(*pathptr);
        }
        free(*pathptr);
        *pathptr = NULL;
    }
}

static int freeShmmapMeta(struct shmmapdata_t *meta, int unlink_files) {
    int retVal = GRAPH_ERR;
    if (meta != NULL) {
        clearMmapFile(&meta->nodefile, unlink_files);
        clearMmapFile(&meta->capfile, unlink_files);
        clearMmapFile(&meta->flowfile, unlink_files);
        meta->nodelen = 0;
        meta->edgelen = 0;
        meta->degree = 0;
        meta->arraylen = 0;
        free(meta);
        retVal = GRAPH_OK;
    }
    return retVal;
}

int shmmapGraphInit(struct graph_t *g) {
    int retVal = GRAPH_ERR;
    if (NULL == g) return GRAPH_ERR;
    if (g->dims == NULL) return GRAPH_ERR;
    enum GRAPHDOMAIN dirtype, imptype, labtype, domaintype, rdtype;
    if (parseTypeFlags(&g->gtype, &dirtype, &imptype, &labtype, &domaintype, &rdtype) == GRAPH_ERR) {
        return GRAPH_ERR;
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
            return GRAPH_ERR;
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
            retVal = GRAPH_OK;
        }
    }
    return retVal;

}

int shmmapGraphAttachReadonly(struct graph_t *g, const struct graph_t *src) {
    if (g == NULL || src == NULL || src->metaImpl == NULL) {
        return GRAPH_ERR;
    }
    struct shmmapdata_t *srcmeta = (struct shmmapdata_t *)src->metaImpl;
    if (srcmeta->roflag == RD_ONLY) {
        return GRAPH_ERR;
    }
    if (srcmeta->nodefile == NULL || srcmeta->capfile == NULL || srcmeta->flowfile == NULL) {
        return GRAPH_ERR;
    }

    struct shmmapdata_t *mapmeta = initShmmapMeta();
    if (mapmeta == NULL) {
        return GRAPH_ERR;
    }
    mapmeta->nodelen = srcmeta->nodelen;
    mapmeta->edgelen = srcmeta->edgelen;
    mapmeta->degree = srcmeta->degree;
    mapmeta->arraylen = srcmeta->arraylen;
    mapmeta->roflag = RD_ONLY;
    mapmeta->nodefile = strdup(srcmeta->nodefile);
    mapmeta->capfile = strdup(srcmeta->capfile);
    mapmeta->flowfile = strdup(srcmeta->flowfile);
    if (mapmeta->nodefile == NULL || mapmeta->capfile == NULL || mapmeta->flowfile == NULL) {
        freeShmmapMeta(mapmeta, 0);
        return GRAPH_ERR;
    }

    g->metaImpl = (void *)mapmeta;
    g->edgeImpl = NULL;
    g->nodeImpl = createNodeMmap(mapmeta, RD_ONLY, PROT_READ, MAP_SHARED);
    g->capImpl = createCapMmap(mapmeta, RD_ONLY, PROT_READ, MAP_SHARED);
    g->flowImpl = createFlowMmap(mapmeta, RD_ONLY, PROT_READ, MAP_SHARED);

    if (g->nodeImpl != NULL && g->capImpl != NULL && g->flowImpl != NULL) {
        return GRAPH_OK;
    }
    return GRAPH_ERR;
}

int shmmapGraphFree(struct graph_t *g) {
    int retVal = GRAPH_OK;
    if (NULL != g && g->metaImpl != NULL) {
        struct shmmapdata_t *mapmeta = (struct shmmapdata_t *) g->metaImpl;
        int unlink_files = (mapmeta->roflag != RD_ONLY);
        if (freeMapMem(g->nodeImpl, mapmeta->arraylen * sizeof(size_t)) != EXIT_SUCCESS) {
            retVal = GRAPH_ERR;
        }
        g->nodeImpl = NULL;
        if (freeMapMem(g->capImpl, mapmeta->arraylen * sizeof(double)) != EXIT_SUCCESS) {
            retVal = GRAPH_ERR;
        }
        g->capImpl = NULL;
        if (freeMapMem(g->flowImpl, mapmeta->arraylen * sizeof(double)) != EXIT_SUCCESS) {
            retVal = GRAPH_ERR;
        }
        g->flowImpl = NULL;
        if (freeShmmapMeta(mapmeta, unlink_files) != GRAPH_OK) {
            retVal = GRAPH_ERR;
        }
        g->metaImpl = NULL;
    }
    return retVal;
}
