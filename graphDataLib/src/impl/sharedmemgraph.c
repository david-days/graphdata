//
// Created by david on 10/21/23.
//

#include <impl/sharedmemgraph.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <cartesian.h>
#include <crudops.h>

static struct shmemdata_t * initShmemMeta() {
    struct shmemdata_t *shmmeta = NULL;
    shmmeta = (struct shmemdata_t *)malloc(sizeof(struct shmemdata_t));
    if (shmmeta != NULL) {
        shmmeta->nodelen = 0;
        shmmeta->edgelen = 0;
        shmmeta->degree = 0;
        shmmeta->arraylen = 0;
        shmmeta->roflag = 0;
        shmmeta->nodekey = -1;
        shmmeta->flowkey = -1;
        shmmeta->capkey = -1;
        shmmeta->nodememid = -1;
        shmmeta->flowmemid = -1;
        shmmeta->capmemid = -1;
    }
    return shmmeta;
}

static int freeShmemMeta(void** metaptr) {
    int retVal = EXIT_FAILURE;
    if (metaptr != NULL && *metaptr != NULL) {
        struct shmemdata_t *shmmeta = (struct shmemdata_t *) *metaptr;
        shmmeta->nodelen = 0;
        shmmeta->edgelen = 0;
        shmmeta->degree = 0;
        shmmeta->arraylen = 0;
        shmmeta->nodekey = -1;
        shmmeta->flowkey = -1;
        shmmeta->capkey = -1;
        shmmeta->nodememid = -1;
        shmmeta->flowmemid = -1;
        shmmeta->capmemid = -1;
        free(*metaptr);
        *metaptr = NULL;
        retVal = EXIT_SUCCESS;
    }
    return retVal;
}

int shmemGraphInit(struct graph_t *g) {
    int retval = EXIT_FAILURE;
    if (NULL == g) return EXIT_FAILURE;
    if (g->dims == NULL) return EXIT_FAILURE;
    enum GRAPHDOMAIN dirtype, imptype, labtype, domaintype, rdtype;
    if (parseTypeFlags(&g->gtype, &dirtype, &imptype, &labtype, &domaintype, &rdtype) == GRAPH_ERR) {
        return EXIT_FAILURE;
    }
    size_t cartlen = cartesianIndexLength(g->dims);
    size_t nlen = cartlen;
    if (labtype == LABELED) {
        nlen = g->labels->labelcount * cartlen;
    }

    // Creator (writable): exclusive create + RW attach.
    // Attacher (RD_ONLY): look up existing keys already stored in meta + RO attach.
    int createFlags = 0;
    int attFlags = 0;

    if (rdtype == RD_ONLY) {
        attFlags = SHM_RDONLY;
    } else {
        createFlags = IPC_CREAT | IPC_EXCL | 0600;
        attFlags = 0;
    }

    if (nlen > 0) {
        struct shmemdata_t *shmmeta = initShmemMeta();
        if (shmmeta == NULL) {
            return EXIT_FAILURE;
        }
        shmmeta->roflag = rdtype;
        shmmeta->nodelen = nlen;
        shmmeta->edgelen = nlen;
        shmmeta->degree = g->dims->dimcount;
        shmmeta->arraylen = nlen * shmmeta->degree;
        g->metaImpl = (void *)shmmeta;
        g->nodeImpl = createNodeShmem(shmmeta, createFlags, attFlags);
        g->edgeImpl = NULL;
        g->capImpl = createCapacityShmem(shmmeta, createFlags, attFlags);
        g->flowImpl = createFlowShmem(shmmeta, createFlags, attFlags);
        if (g->nodeImpl != NULL && g->capImpl != NULL && g->flowImpl != NULL)
            retval = EXIT_SUCCESS;
    }

    return retval;
}

int shmemGraphAttachReadonly(struct graph_t *g, const struct graph_t *src) {
    if (g == NULL || src == NULL || src->metaImpl == NULL) {
        return EXIT_FAILURE;
    }
    struct shmemdata_t *srcmeta = (struct shmemdata_t *)src->metaImpl;
    if (srcmeta->roflag == RD_ONLY) {
        return EXIT_FAILURE;
    }
    if (srcmeta->nodekey == (key_t)-1 || srcmeta->capkey == (key_t)-1 || srcmeta->flowkey == (key_t)-1) {
        return EXIT_FAILURE;
    }

    struct shmemdata_t *shmmeta = initShmemMeta();
    if (shmmeta == NULL) {
        return EXIT_FAILURE;
    }
    shmmeta->nodelen = srcmeta->nodelen;
    shmmeta->edgelen = srcmeta->edgelen;
    shmmeta->degree = srcmeta->degree;
    shmmeta->arraylen = srcmeta->arraylen;
    shmmeta->roflag = RD_ONLY;
    shmmeta->nodekey = srcmeta->nodekey;
    shmmeta->capkey = srcmeta->capkey;
    shmmeta->flowkey = srcmeta->flowkey;

    g->metaImpl = (void *)shmmeta;
    g->edgeImpl = NULL;
    g->nodeImpl = createNodeShmem(shmmeta, 0, SHM_RDONLY);
    g->capImpl = createCapacityShmem(shmmeta, 0, SHM_RDONLY);
    g->flowImpl = createFlowShmem(shmmeta, 0, SHM_RDONLY);

    if (g->nodeImpl != NULL && g->capImpl != NULL && g->flowImpl != NULL) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


int shmemGraphFree(struct graph_t *g) {
    int retval = EXIT_SUCCESS;
    if (NULL != g && g->metaImpl != NULL) {
        struct shmemdata_t *shmmeta = (struct shmemdata_t *) g->metaImpl;
        int remove_segment = (shmmeta->roflag != RD_ONLY);
        retval = retval | freeShmem(g->nodeImpl, shmmeta->nodememid, remove_segment);
        g->nodeImpl = NULL;
        retval = retval | freeShmem(g->capImpl, shmmeta->capmemid, remove_segment);
        g->capImpl = NULL;
        retval = retval | freeShmem(g->flowImpl, shmmeta->flowmemid, remove_segment);
        g->flowImpl = NULL;
        retval = retval | freeShmemMeta(&(g->metaImpl));
        g->metaImpl = NULL;
    }
    return retval;
}
