//
// Created by david on 5/20/25.
//

#include <sharedmmapgraph.h>
#include <util/memmgt.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

static char* nodeFile = NULL;
static char* flowFile = NULL;
static char* capFile = NULL;

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

void * createNodeShmem(struct shmemdata_t *shmmeta, int createflags, int attflags) {
    key_t nodekey;
    size_t nodeCount = shmmeta->nodelen * shmmeta->degree;
    if (createflags == 0 && attflags == 0) {
        //create RW shared memory segment
        nodekey = ftok(nodeFile, rand());
        if (nodeFile == NULL) {
            char* nodeTemplate = "nodeXXXXXX";
            int nodefd = mkstemp(nodeTemplate);
            if (nodefd == -1) {
                perror("mkstemp()");
                printErrMessage();
                return NULL;
            }
            nodeFile = nodeTemplate;
        }
        if (nodekey == -1) {
            perror("ftok()");
            printErrMessage();
            return NULL;
        }
        shmmeta->nodekey = nodekey;
    } else {
        //create a readonly connection
        nodekey = shmmeta->nodekey;
    }

    int nodeid = createShmem(nodeCount, nodekey, createflags);
    if (nodeid == -1) {
        perror("createShmem()");
        printErrMessage();
        return NULL;
    }
    shmmeta->nodememid = nodeid;

    void * nodeMem = shmat(nodeid, NULL, attflags);
    if (nodeMem == (void *) -1) {
        perror("shmat()");
        printErrMessage();
        return NULL;
    }
    return nodeMem;
}

void * createFlowShmem(struct shmemdata_t *shmmeta, int createflags, int attflags) {
    key_t flowkey;
    size_t flowlen = sizeof(double *) * shmmeta->nodelen;
    size_t flowcount = flowlen * shmmeta->degree;
    if (createflags == 0 && attflags == 0) {
        flowkey = ftok(flowFile, rand());
        if (flowFile == NULL) {
            char* flowTemplate = "flowXXXXXX";
            int flowfd = mkstemp(flowTemplate);
            if (flowfd == -1) {
                perror("mkstemp()");
                printErrMessage();
                return NULL;
            }
            flowFile = flowTemplate;

            if (flowkey == -1) {
                perror("ftok()");
                printErrMessage();
                return NULL;
            }
            shmmeta->flowkey = flowkey;
        }
    } else {
        flowkey = shmmeta->flowkey;
    }


    int flowid = createShmem(flowcount, flowkey, createflags);
    if (flowid == -1) {
        perror("createShmem()");
        printErrMessage();
        return NULL;
    }
    shmmeta->flowmemid = flowid;
    void * flowMem = shmat(flowid, NULL, attflags);
    if (flowMem == (void *) -1) {
        perror("shmat()");
        printErrMessage();
        return NULL;
    }
    return flowMem;
}

void * createCapacityShmem(struct shmemdata_t *shmmeta, int createflags, int attflags) {
    key_t capkey;
    size_t caplen = sizeof(double *) * shmmeta->nodelen;
    size_t capcount = caplen * shmmeta->degree;
    if (createflags == 0 && attflags == 0) {
        capkey = ftok(capFile, rand());
        if (capFile == NULL) {
            char* capTemplate = "capXXXXXX";
            int capfd = mkstemp(capTemplate);
            if (capfd == -1) {
                perror("mkstemp()");
                printErrMessage();
                return NULL;
            }
            capFile = capTemplate;
            if (capkey == -1) {
                perror("ftok()");
                printErrMessage();
                return NULL;
            }
            shmmeta->capkey = capkey;
        }
    } else {
        capkey = shmmeta->capkey;
    }
    int capid = createShmem(capcount, capkey, createflags);
    if (capid == -1) {
        perror("createShmem()");
        printErrMessage();
        return NULL;
    }
    shmmeta->capmemid = capid;
    void * capMem = shmat(capid, NULL, attflags);
    if (capMem == (void *) -1) {
        perror("shmat()");
        printErrMessage();
        return NULL;
    }
    return capMem;
}

int createShmem(size_t memLen, key_t key, int shmflags) {
    int shmId = shmget(key, memLen, shmflags);
    return shmId;
}

int freeShmem(key_t key, int shmid) {
    int retval = EXIT_FAILURE;
    int detval = shmdt(key);
    if (detval == -1) {
        perror("shmdt()");
        printErrMessage();
    } else {
        retval = EXIT_SUCCESS;
    }
    int rmval = shmctl(shmid, IPC_RMID, NULL);
    if (rmval == -1) {
        perror("shmctl()");
        printErrMessage();
        retval = EXIT_FAILURE;
    }
    return retval;
}


void * createNodeMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags) {
    size_t nodeCount = shmmap->nodelen * shmmap->degree;
    char * nodeFile = shmmap->nodefile;
    if (nodeFile == NULL) {
        //We'll need to create the node file
        nodeFile = "mmapNodeXXXXXX";
        int nodefd = mkstemp(nodeFile);
        if (nodefd == -1) {
            perror("mkstemp()");
            return NULL;
        }
        close(nodefd);
        shmmap->nodefile = nodeFile;
    }
    void * mmapAddr = createMmap(nodeCount, nodeFile, roflag, protflags, mapflags);
    if (mmapAddr > 0 && roflag != RD_ONLY) {
        size_t *nodeVals = (size_t *) mmapAddr;
        for (size_t *p = nodeVals; p < nodeVals + nodeCount; p++) {
            *p = 0;
        }
    }
    return mmapAddr;
}

void * createCapMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags) {
    size_t caplen = sizeof(double *) * shmmap->nodelen;
    size_t capCount = caplen * shmmap->degree;
    char * capFile = shmmap->capfile;
    if (capFile == NULL) {
        //We'll need to create the node file
        capFile = "mmapCapacityXXXXXX";
        int capfd = mkstemp(capFile);
        if (capfd == -1) {
            perror("mkstemp()");
            return NULL;
        }
        close(capfd);
        shmmap->capfile = capFile;
    }
    void * mmapAddr = createMmap(capCount, capFile, roflag, protflags, mapflags);
    if (mmapAddr > 0 && roflag != RD_ONLY) {
        double *capVals = (double *) mmapAddr;
        for (double *p = capVals; p < capVals + capCount; p++) {
            *p = 0.0;
        }
    }
    return mmapAddr;
}

void * createFlowMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags) {
    size_t flowlen = sizeof(double *) * shmmap->nodelen;
    size_t flowCount = flowlen * shmmap->degree;
    char * flowFile = shmmap->flowfile;
    if (flowFile == NULL) {
        //We'll need to create the node file
        flowFile = "mmapFlowXXXXXX";
        int flowfd = mkstemp(flowFile);
        if (flowfd == -1) {
            perror("mkstemp()");
            return NULL;
        }
        close(flowfd);
        shmmap->flowfile = flowFile;
    }
    void * mmapAddr = createMmap(flowCount, flowFile, roflag, protflags, mapflags);
    if (mmapAddr > 0 && roflag != RD_ONLY) {
        double *flowVals = (double *) mmapAddr;
        for (double *p = flowVals; p < flowVals + flowCount; p++) {
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
        //opening the file failed
        fprintf(stderr, "Error opening %s\n", fpath);
        perror("open()");
        printErrMessage();
        return NULL;
    }
    if (roflag != RD_ONLY) {
        size_t result = lseek(fd, memLen-1, SEEK_SET);
        if (result == -1) {
            close(fd);
            perror("Error calling lseek() to 'stretch' the file");
            return NULL;
        }

        result = write(fd, "", 1);
        if (result != 1) {
            close(fd);
            perror("Error writing last byte of the file");
            exit(EXIT_FAILURE);
        }
    }
    // now the file is ready to be mmapped
    void *map = mmap(0, memLen, protflags, mapflags, fd, 0);
    // can close without causing issues.
    close(fd);
    if (map == MAP_FAILED) {
        perror("Error mmapping the file");
        return NULL;
    }
    return map;
}

int freeMapMem(void * mapaddr, size_t mlen) {
    int retval = EXIT_FAILURE;

    int unmapped = munmap(mapaddr, mlen);
    if (unmapped == -1) {
        perror("munmap()");
    } else {
        retval = EXIT_SUCCESS;
    }
    return retval;
}
