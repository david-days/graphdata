//
// Created by david on 5/20/25.
//

#ifndef MEMMGT_H
#define MEMMGT_H

#include <sys/types.h>
#include <sharedmemgraph.h>

/**
 * Print message associated with current errno value.
 */
void printErrMessage();

/**
 * Creates or attaches to the shared memory segment for node data, in accordance with the given flags
 * @param shmmeta Metadata containing initial conditions and requirements for the shared memory
 * @param shmflags Creation/attachment flags to be used for the requested shared memory
 * @return pointer to the shared memory segment if successful; otherwise returns NULL
 */
void * createNodeShmem(struct shmemdata_t *shmmeta, int createflags, int attflags);

/**
 * Creates or attaches to the shared memory segment for flow data, in accordance with the given flags.
 * @param shmmeta Metadata containing initial conditions and requirements for the shared memory
 * @param shmflags Creation/attachment flags to be used fo rthe requested shared memory
 * @return pointer to the shared memory segment if successful; otherwise, returns NULL
 */
void * createFlowShmem(struct shmemdata_t *shmmeta, int createflags, int attflags);

/**
 * Creates or attaches to the shared memory segment for capacity data, in accordance with the given flags.
 * @param shmmeta Metadata containing initial conditions and requirements for the shared memory
 * @param shmflags Creation/attachment flags to be used fo rthe requested shared memory
 * @return pointer to the shared memory segment if successful; otherwise, returns NULL
 */
void * createCapacityShmem(struct shmemdata_t *shmmeta, int createflags, int attflags);

/**
 * Utility function to create or find a shared memory segment.
 * @param memLen Length in size_t of the shared memory
 * @param key key identifier of the shared memory (existing or to-be-created)
 * @param shmflags flags to be used for this operation
 * @return id of the shared memory segment, if successful; otherwise, returns -1.
 */
int createShmem(size_t memLen, key_t key, int shmflags);

/**
 * Free the shared memory segment described by the given key and id values
 * @param key shared memory key
 * @param shmid shared memory id
 * @return EXIT_SUCCESS if succeeded; otherwise EXIT_FAILED
 */
int freeShmem(key_t key, int shmid);

/**
 * Create node mmap using the given base info and flags
 * @param shmmap graph implementation details
 * @param protflags mmap PROT_* flags to use
 * @param mapflags mmap MAP_* flags to use
 * @return pointer to memory address if successful; otherwise value <= 0
 */
void * createNodeMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags);

/**
 * Create the capacity array implementation
 * @param shmmap graph implementation details
 * @param protflags mmap PROT_* flags to use
 * @param mapflags mmap MAP_* flags to use
 * @return pointer to memory address if successful; otherwise value <= 0
 */
void * createCapMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags);

/**
 * Create the flow array implementation
 * @param shmmap graph implementation details
 * @param protflags mmap PROT_* flags to use
 * @param mapflags mmap MAP_* flags to use
* @return pointer to memory address if successful; otherwise value <= 0
 */
void * createFlowMmap(struct shmmapdata_t * shmmap, enum GRAPHDOMAIN roflag, int protflags, int mapflags);

/**
 * Create a specific memory mapping
 * @param memLen size_t length to use
 * @param fpath file path template (mkstemp format, with last 6 characters XXXXXX) to be mapped to
 * @param protflags PROT_* flags to use
 * @param mapflags MAP_* flags to use
 * @return pointer to mmap address; pointer is <= 0 if there is an error or problem
 */
void * createMmap(size_t memLen, char *fpath, enum GRAPHDOMAIN roflag, int protflags, int mapflags);

/**
 * clear a memory mapping at the given address
 * @param mapaddr mmap address
 * @param mlen length of the mapped memory
 * @return EXIT_SUCCESS if the mapping was clears; otherwise, EXIT_FAILURE
 */
int freeMapMem(void * mapaddr, size_t mlen);

#endif //MEMMGT_H
