//
// Created by david on 10/23/23.
//

#include <graphData.h>
#include <util/hashtable.h>
#include <util/hashes.h>
#include <stdlib.h>



// utility function definitions
/**
 * Initialize the internals of the hashtable structure
 * @param ht Hashtable pointer to be initialized
 * @param htPrime Prime number to be used for capacity and modulus
 */
void init_hashtable_items(struct hash_table *ht, size_t htPrime);

/**
 * Given a reference to a (possible) ht_item linked list, find the entry with the given hash value
 * @param chain ht_item linked list
 * @param hashVal size_t value identifying the entry
 * @return pointer to the entry value, if found; otherwise, NULL
 */
void * itemChain_find(struct ht_item *chain, size_t hashVal);

// end utility function definitions

struct hash_table * init_hashtable(size_t exp_capacity, hashFunc hashFunction) {
    struct hash_table *ht = NULL;
    ht = (struct hash_table *)malloc(sizeof(struct hash_table));
    if (ht != NULL) {
        size_t capPrime = maxEratosthenesPrime(&exp_capacity);
        do {
            capPrime = nextPrime(capPrime);
        } while (capPrime < exp_capacity);
        ht->hash = hashFunction;
        init_hashtable_items(ht, capPrime);
    }
    return ht;
}

int hashtable_add(void *keyVal, void *valVal, struct hash_table *ht) {
    int retVal = hashtable_check(ht);
    if (retVal > 0) {
        size_t keyHash = ht->hash(keyVal);
        size_t hashMod = keyHash % ht->modulo;
        struct ht_item *keyItem = (struct ht_item *)malloc(sizeof(ht_item));
        struct ht_item *valItem = (struct ht_item *)malloc(sizeof(ht_item));
        
        if (keyItem != NULL && valItem != NULL) {
            //set up key
            keyItem->hashVal = keyHash;
            keyItem->itemVal = keyVal;
            keyItem->next = NULL;
            keyItem->prev = NULL;

            if (ht->keys == NULL) {
                //first item to be added
                ht->keys[hashMod] = keyItem;
            } else {
                struct ht_item *currKey = (struct ht_item *)ht->keys[hashMod];
                struct ht_item *nextKey;
                while (currKey->next != NULL) {
                    nextKey = currKey->next;
                    currKey = nextKey;
                }
                currKey->next = keyItem;
                keyItem->prev = currKey;
            }
            
            //set up val
            valItem->hashVal = keyHash;
            valItem->itemVal = valVal;
            valItem->next = NULL;
            valItem->prev = NULL;

            if (ht->values == NULL) {
                //first item to be added
                ht->values[hashMod] = valItem;
            } else {
                struct ht_item *currVal = (struct ht_item *)ht->values[hashMod];
                struct ht_item *nextVal;
                while (currVal->next != NULL) {
                    nextVal = currVal->next;
                    currVal = nextVal;
                }
                currVal->next = valItem;
                valItem->prev = currVal;
            }
            
            ht->count++;
            
            retVal = 1;
        } else {
            retVal = -1;
        }
    }
    return retVal;
}

void * hashtable_get(void *keyVal, struct hash_table *ht) {
    void *retVal = NULL;
    size_t keyHash = ht->hash(keyVal);
    size_t keyMod = keyHash % ht->modulo;
    if (keyMod > ht->capacity) return retVal;
    //is this a valid entry?
    struct ht_item *keyChain = ht->keys[keyMod];
    void *keyEntry = itemChain_find(keyChain, keyHash);
    if (keyEntry == NULL) return retVal;
    //entry is valid--find the matching value
    struct ht_item *valChain = ht->values[keyMod];
    retVal = itemChain_find(valChain, keyHash);
    return retVal;
}

void * hashtable_remove(void *keyVal, struct hash_table *ht) {
    void *retVal = NULL;
    size_t keyHash = ht->hash(keyVal);
    size_t keyMod = keyHash % ht->modulo;
    if (keyMod > ht->capacity) return retVal;
    //is this a valid entry?
    struct ht_item *keyChain = ht->keys[keyMod];
    void *keyEntry = itemChain_find(keyChain, keyHash);
    if (keyEntry == NULL) return retVal;
    //entry is valid--find the matching key
    struct ht_item *currKey = ht->keys[keyMod];
    struct ht_item *nextKey = currKey->next;
    uint keyFound = 0;
    if (currKey->hashVal == keyHash) {
        keyFound = 1;
        if (nextKey != NULL) nextKey->prev = NULL;
        ht->keys[keyMod] = nextKey;
        free(currKey);
    } else {
        struct ht_item *prevKey = NULL;
        while (currKey->next != NULL && keyFound == 0) {
            prevKey = currKey;
            currKey = nextKey;
            nextKey = currKey->next;
            if (currKey->hashVal == keyHash) {
                keyFound = 1;
                //close up chain
                if (prevKey != NULL) prevKey->next = nextKey;
                if (nextKey != NULL) nextKey->prev = prevKey;
                free(currKey);
            }
        }
    }
    //find matching value
    struct ht_item *currVal = ht->values[keyMod];
    struct ht_item *nextVal = currVal->next;
    if (currVal->hashVal == keyHash) {
        retVal = currVal->itemVal;
        if (nextVal != NULL) nextVal->prev = NULL;
        ht->values[keyMod] = nextVal;
        free(currVal);
        ht->count--;
    } else {
        struct ht_item *prevVal = NULL;
        while (currVal->next != NULL && retVal == NULL) {
            prevVal = currVal;
            currVal = nextVal;
            nextVal = currVal->next;
            if (currVal->hashVal == keyHash) {
                retVal = currVal->itemVal;
                //close up chain
                if (prevVal != NULL) prevVal->next = nextVal;
                if (nextVal != NULL) nextVal->prev = prevVal;
                free(currVal);
                ht->count--;
            }
        }
    }
    
    return retVal;
}

int hashtable_free(struct hash_table *ht) {
    int retval = 1;
    //Free up the underlying arrays, first
    free(ht->keys);
    free(ht->values);
    //now free up the core structure
    free(ht);
    return retval;
}


struct hash_table * hashtable_grow(struct hash_table *ht) {
    struct hash_table *nextHt = NULL;
    nextHt = (struct hash_table *)malloc(sizeof(struct hash_table));
    if (nextHt != NULL) {
        size_t capPrime = nextPrime(ht->modulo);
        nextHt->hash = ht->hash;
        init_hashtable_items(nextHt, capPrime);
    }
    int xfr = hashtable_move(ht, nextHt);
    if (xfr <= 0) {
        //something went wrong--free up all the memory
        hashtable_free(nextHt);
        nextHt = NULL;
    }
    return nextHt;
}

int hashtable_check(struct hash_table *ht) {
    int capacious = 1;
    size_t room = ht->capacity - ht->count;
    size_t tithe = ht->capacity / 10;
    if (room < 10 || room <= tithe) {
        capacious = 0;
    }
    return capacious;
}

int hashtable_move(struct hash_table *oldHt, struct hash_table *nextHt) {
    int xfr_success = 1;
    size_t oldCap = oldHt->capacity;
    size_t nextCap = nextHt->capacity;
    
    void *currKey;
    void *currVal;
    struct ht_item *keyChain;
    struct ht_item *currLink;
    struct ht_item *nextLink;
    for (size_t i=0; i < oldCap; i++) {
        keyChain = oldHt->keys[i];
        if (keyChain != NULL) {
            //we have an entry key
            currLink = keyChain;
            nextLink = currLink->next;
            do {
                //move the bin items pointers over to the next hashtable
                currKey = currLink->itemVal;
                currVal = hashtable_get(currKey, oldHt);
                if (currVal != NULL) {
                    hashtable_add(currKey, currVal, nextHt);
                }
                currLink = nextLink;
                if (currLink != NULL) {
                    nextLink = currLink->next;
                }
            } while (currLink != NULL);
        }
    }
    
    //Now reverse the process to empty out the old one
    for (size_t j=0; j < nextCap; j++) {
        keyChain = nextHt->keys[j];
        if (keyChain != NULL) {
            //we have an entry key
            currLink = keyChain;
            nextLink = currLink->next;
            do {
                //remove the items from the old hashtable
                currKey = currLink->itemVal;
                if (currVal != NULL) {
                    hashtable_remove(currKey, oldHt);
                }
                currLink = nextLink;
                if (currLink != NULL) {
                    nextLink = currLink->next;
                }
            } while (currLink != NULL);
        }
    }
    
    return xfr_success;
}

//internal utility functions

void init_hashtable_items(struct hash_table *ht, size_t htPrime) {
    ht->count = 0;
    ht->modulo = htPrime;
    ht->capacity = htPrime;
    ht->keys = calloc(ht->capacity, sizeof(struct ht_item **));
    ht->values = calloc(ht->capacity, sizeof(struct ht_item **));
}


void * itemChain_find(struct ht_item *link, size_t hashVal) {
    void * retVal = NULL;
    //degenerate case
    if (link->hashVal == hashVal) retVal = link->itemVal;
    struct ht_item *curr = link;
    struct ht_item *next = curr->next;
    while (curr->next != NULL && retVal == NULL) {
        curr = next;
        next = curr->next;
        if (curr->hashVal == hashVal) retVal = curr->itemVal;
    }
    return retVal;
}
// end internal utility functions




