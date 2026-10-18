//
// Created by david on 5/20/25.
//

#include <sharedmmapgraph.h>
#include <sharedmemgraph.h>
#include <util/memmgt.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

void printErrMessage() {
    switch (errno) {
        case EACCES:
            fprintf(stderr, "The user does not have permission to access the shared memory segment, and does not have the CAP_IPC_OWNER capability in the user namespace that governs its IPC namespace.");
            break;
        case EEXIST:
            fprintf(stderr, "IPC_CREAT and IPC_EXCL were specified in shmflg, but a shared memory segment already exists for key.");
            break;
        case EINVAL:
            fprintf(stderr, "A new segment was to be created and size is less than SHMMIN or greater than SHMMAX.");
            fprintf(stderr, "Alternatively, a segment for the given key exists, but size is greater than the size of that segment.");
            break;
        case ENFILE:
            fprintf(stderr, "The system-wide limit on the total number of open files has been reached.");
            break;
        case ENOENT:
            fprintf(stderr, "No segment exists for the given key, and IPC_CREAT was not specified.");
            break;
        case ENOMEM:
            fprintf(stderr, "No memory could be allocated for segment overhead.");
            break;
        case ENOSPC:
            fprintf(stderr, "All possible shared memory IDs have been taken (SHMMNI), or allocating a segment of the requested size would cause the system to exceed the system-wide limit on shared memory (SHMALL).");
            break;
        case EPERM:
            fprintf(stderr, "The  SHM_HUGETLB  flag  was  specified,  but  the  caller  was  not  privileged  (did  not  have  the  CAP_IPC_LOCK  capability)  and  is  not  a  member  of  the  sysctl_hugetlb_shm_group  group.");
            break;
        default:
            fprintf(stderr, "Error %d when attempting to create shared memory segment", errno);
    }
}

/**
 * Create a unique temp path for ftok(), then unlink the file (key stays valid).
 */
static key_t makeShmemKey(char projid) {
    char *path = strdup("/tmp/graphdataXXXXXX");
    if (path == NULL) {
        return (key_t) -1;
    }
    int fd = mkstemp(path);
    if (fd == -1) {
        perror("mkstemp()");
        printErrMessage();
        free(path);
        return (key_t) -1;
    }
    close(fd);

    key_t key = ftok(path, projid);
    unlink(path);
    free(path);

    if (key == (key_t) -1) {
        perror("ftok()");
        printErrMessage();
    }
    return key;
}

static void * attachShmem(size_t nbytes, key_t *key_inout, int *memid_out,
                          int createflags, int attflags, char projid) {
    key_t key;
    if (createflags & IPC_CREAT) {
        key = makeShmemKey(projid);
        if (key == (key_t) -1) {
            return NULL;
        }
        *key_inout = key;
    } else {
        key = *key_inout;
        if (key == (key_t) -1) {
            fprintf(stderr, "attachShmem(): missing shared-memory key for attach\n");
            return NULL;
        }
    }

    int shmid = createShmem(nbytes, key, createflags);
    if (shmid == -1) {
        perror("createShmem()");
        printErrMessage();
        return NULL;
    }
    *memid_out = shmid;

    void *mem = shmat(shmid, NULL, attflags);
    if (mem == (void *) -1) {
        perror("shmat()");
        printErrMessage();
        shmctl(shmid, IPC_RMID, NULL);
        return NULL;
    }

    return mem;
}

void * createNodeShmem(struct shmemdata_t *shmmeta, int createflags, int attflags) {
    size_t nbytes = shmmeta->arraylen * sizeof(size_t);
    void *mem = attachShmem(nbytes, &shmmeta->nodekey, &shmmeta->nodememid,
                            createflags, attflags, 'N');
    if (mem != NULL && (createflags & IPC_CREAT) && !(attflags & SHM_RDONLY)) {
        size_t *slots = (size_t *)mem;
        for (size_t i = 0; i < shmmeta->arraylen; i++) {
            slots[i] = GRAPH_SLOT_EMPTY;
        }
    }
    return mem;
}

void * createFlowShmem(struct shmemdata_t *shmmeta, int createflags, int attflags) {
    size_t nbytes = shmmeta->arraylen * sizeof(double);
    void *mem = attachShmem(nbytes, &shmmeta->flowkey, &shmmeta->flowmemid,
                            createflags, attflags, 'F');
    if (mem != NULL && (createflags & IPC_CREAT) && !(attflags & SHM_RDONLY)) {
        memset(mem, 0, nbytes);
    }
    return mem;
}

void * createCapacityShmem(struct shmemdata_t *shmmeta, int createflags, int attflags) {
    size_t nbytes = shmmeta->arraylen * sizeof(double);
    void *mem = attachShmem(nbytes, &shmmeta->capkey, &shmmeta->capmemid,
                            createflags, attflags, 'C');
    if (mem != NULL && (createflags & IPC_CREAT) && !(attflags & SHM_RDONLY)) {
        memset(mem, 0, nbytes);
    }
    return mem;
}

int createShmem(size_t memLen, key_t key, int shmflags) {
    return shmget(key, memLen, shmflags);
}

int freeShmem(void *addr, int shmid, int remove_segment) {
    int retval = EXIT_SUCCESS;

    if (addr != NULL && addr != (void *) -1) {
        if (shmdt(addr) == -1) {
            perror("shmdt()");
            printErrMessage();
            retval = EXIT_FAILURE;
        }
    }

    if (remove_segment) {
        if (shmid >= 0) {
            if (shmctl(shmid, IPC_RMID, NULL) == -1) {
                perror("shmctl()");
                printErrMessage();
                retval = EXIT_FAILURE;
            }
        } else {
            retval = EXIT_FAILURE;
        }
    }
    return retval;
}

static char * makeMmapPath(const char *template) {
    char *path = strdup(template);
    if (path == NULL) {
        return NULL;
    }
    int fd = mkstemp(path);
    if (fd == -1) {
        perror("mkstemp()");
        free(path);
        return NULL;
    }
    close(fd);
    return path;
}

void * createNodeMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags) {
    size_t nbytes = shmmap->arraylen * sizeof(size_t);
    if (shmmap->nodefile == NULL) {
        shmmap->nodefile = makeMmapPath("/tmp/mmapNodeXXXXXX");
        if (shmmap->nodefile == NULL) {
            return NULL;
        }
    }
    void * mmapAddr = createMmap(nbytes, shmmap->nodefile, roflag, protflags, mapflags);
    if (mmapAddr != NULL && roflag != RD_ONLY) {
        size_t *nodeVals = (size_t *) mmapAddr;
        for (size_t *p = nodeVals; p < nodeVals + shmmap->arraylen; p++) {
            *p = GRAPH_SLOT_EMPTY;
        }
    }
    return mmapAddr;
}

void * createCapMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags) {
    size_t nbytes = shmmap->arraylen * sizeof(double);
    if (shmmap->capfile == NULL) {
        shmmap->capfile = makeMmapPath("/tmp/mmapCapacityXXXXXX");
        if (shmmap->capfile == NULL) {
            return NULL;
        }
    }
    void * mmapAddr = createMmap(nbytes, shmmap->capfile, roflag, protflags, mapflags);
    if (mmapAddr != NULL && roflag != RD_ONLY) {
        double *capVals = (double *) mmapAddr;
        for (double *p = capVals; p < capVals + shmmap->arraylen; p++) {
            *p = 0.0;
        }
    }
    return mmapAddr;
}

void * createFlowMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags) {
    size_t nbytes = shmmap->arraylen * sizeof(double);
    if (shmmap->flowfile == NULL) {
        shmmap->flowfile = makeMmapPath("/tmp/mmapFlowXXXXXX");
        if (shmmap->flowfile == NULL) {
            return NULL;
        }
    }
    void * mmapAddr = createMmap(nbytes, shmmap->flowfile, roflag, protflags, mapflags);
    if (mmapAddr != NULL && roflag != RD_ONLY) {
        double *flowVals = (double *) mmapAddr;
        for (double *p = flowVals; p < flowVals + shmmap->arraylen; p++) {
            *p = 0.0;
        }
    }
    return mmapAddr;
}

void * createMmap(size_t memLen, char *fpath, enum GRAPHDOMAIN roflag, int protflags, int mapflags) {
    mode_t writemode = 0600;
    int oflags = O_RDWR | O_CREAT | O_TRUNC;
    if (roflag == RD_ONLY) {
        writemode = 0400;
        oflags = O_RDONLY;
    }
    int fd = open(fpath, oflags, writemode);
    if (fd == -1) {
        fprintf(stderr, "Error opening %s\n", fpath);
        perror("open()");
        printErrMessage();
        return NULL;
    }
    if (roflag != RD_ONLY) {
        off_t result = lseek(fd, (off_t) memLen - 1, SEEK_SET);
        if (result == -1) {
            close(fd);
            perror("Error calling lseek() to 'stretch' the file");
            return NULL;
        }

        if (write(fd, "", 1) != 1) {
            close(fd);
            perror("Error writing last byte of the file");
            return NULL;
        }
    }
    void *map = mmap(0, memLen, protflags, mapflags, fd, 0);
    close(fd);
    if (map == MAP_FAILED) {
        perror("Error mmapping the file");
        return NULL;
    }
    return map;
}

int freeMapMem(void * mapaddr, size_t mlen) {
    int retval = EXIT_FAILURE;

    if (mapaddr == NULL || mapaddr == MAP_FAILED) {
        return EXIT_FAILURE;
    }

    int unmapped = munmap(mapaddr, mlen);
    if (unmapped == -1) {
        perror("munmap()");
    } else {
        retval = EXIT_SUCCESS;
    }
    return retval;
}
