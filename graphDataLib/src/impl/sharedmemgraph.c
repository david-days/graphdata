//
// Created by david on 10/21/23.
//

#include <impl/sharedmemgraph.h>
#include <errno.h>
#include <bits/ipc.h>
#include <bits/shm.h>

#include <cartesian.h>
#include <crudops.h>

static struct shmemdata_t * initShmemMeta() {
    struct shmemdata_t *shmmeta = NULL;
    shmmeta = (struct shmemdata_t *)malloc(sizeof(struct shmemdata_t));
    if (shmmeta != NULL) {
        shmmeta->nodelen = 0;
        shmmeta->degree = 0;
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
    if (*metaptr != NULL) {
        struct shmemdata_t *shmmeta = (struct shmemdata_t *) metaptr;
        shmmeta->nodelen = 0;
        shmmeta->degree = 0;
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
    if (parseTypeFlags(&g->gtype, &dirtype, &imptype, &labtype, &domaintype, &rdtype) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    size_t cartlen = cartesianIndexLength(g->dims);
    size_t nlen = cartlen;
    if (labtype == LABELED) {
        nlen = g->labels->labelcount * cartlen;
    }

    // generate the shared memory flags, based on requested usage
    int createFlags = 0;
    int attFlags = 0;

    if (rdtype != RD_ONLY) {
        createFlags = IPC_CREAT | IPC_EXCL;
        attFlags = SHM_RDONLY;
    }

    if (nlen > 0) {
        struct shmemdata_t *shmmeta = initShmemMeta();
        shmmeta->roflag = rdtype;
        shmmeta->nodelen = nlen;
        shmmeta->degree = g->dims->dimcount;
        shmmeta->arraylen = nlen * shmmeta->degree;
        g->nodeImpl = createNodeShmem(shmmeta, createFlags, attFlags);
        g->edgeImpl = NULL;
        g->capImpl = createCapacityShmem(shmmeta, createFlags, attFlags);
        g->flowImpl = createFlowShmem(shmmeta, createFlags, attFlags);
        g->metaImpl = (void *)shmmeta;
        if (g->nodeImpl != NULL && g->capImpl != NULL && g->flowImpl != NULL)
            retval = EXIT_SUCCESS;
    }

    return retval;
}


int shmemGraphFree(struct graph_t *g) {
    int retval = EXIT_SUCCESS;
    if (NULL != g) {
        struct shmemdata_t *shmmeta = (struct shmemdata_t *) g->metaImpl;
        retval = retval | freeShmem(shmmeta->nodekey, shmmeta->nodememid);
        g->nodeImpl = NULL;
        retval = retval | freeShmem(shmmeta->capkey, shmmeta->capmemid);
        g->capImpl = NULL;
        retval = retval | freeShmem(shmmeta->flowkey, shmmeta->flowmemid);
        g->flowImpl = NULL;
        retval = retval | freeShmemMeta(&(g->metaImpl));
        g->metaImpl = NULL;
    }
    return retval;
}