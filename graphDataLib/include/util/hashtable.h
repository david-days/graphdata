//
// Created by david on 10/23/23.
//

#ifndef GRAPHDATA_HASHTABLE_H
#define GRAPHDATA_HASHTABLE_H

#include <graphData.h>
#include <hashes.h>

/**
 * @brief Function pointer for hash function to use with this table and key types
 *
 * @param key pointer to key value for which the hash value is needed
  * @return hash value for the given key
 * retrieval
 */
typedef size_t (*hashFunc)(void *key);

/**
 * Linked-list structure to hold items in hashtable bins (for collision management)
 */
struct ht_item {
    size_t hashVal;
    void * itemVal;
    struct ht_item *next;
    struct ht_item *prev;
} ht_item;

/**
 * Hashtable structure
 */
struct hash_table {
    struct ht_item **keys;
    struct ht_item **values;
    hashFunc hash;
    size_t capacity;
    size_t count;
    size_t modulo;
} hash_table;

/**
 * @brief Initialize a hashtable with the expected capacity and the given hash function
 * @param exp_capacity Expected capacity.  Actual capacity will be a prime number this value or higher
 * @param hashFunction Hash function to use on key entries
 * @return 
 */
struct hash_table * init_hashtable(size_t exp_capacity, hashFunc hashFunction);

/**
 * @brief  Add a key-value pair to the hashtable
 * @param keyVal Key for the hashtable entry
 * @param valVal value to be entered
 * @param ht Hashtable to which the key-value pair are added
 * @return 1 if successful, otherwise 0
 */
int hashtable_add(void *keyVal, void *valVal, struct hash_table *ht);

/**
 * @brief retrieve the value, from the given hashtable, that matches the given key
 * @param keyVal Key entry
 * @param ht Hashtable to be checked
 * @return void * reference to the value if found; otherwise, NULL
 */
void * hashtable_get(void *keyVal, struct hash_table *ht);

/**
 * @brief Remove the key-value pair from the hashtable
 * 
 * @param keyVal key identifier of the pair 
 * @param ht hashtable to be checked
 * @return The value if the key is a valid entry identifier; otherwise, NULL
 */
void * hashtable_remove(void *keyVal, struct hash_table *ht);

/**
 * @brief  Free up the memory used by the empty(!) hashtable
 * @param ht Hashtable whose memory is to be freed
 * @return if successful, 1; otherwise return 0
 */
int hashtable_free(struct hash_table *ht);

/**
 * @brief Create a hashtable with the next designated/available size, and all the entries moved over 
 * (removed from the old hashtable)
 * @param ht hashtable to be expanded from and cleared 
 * @return pointer to a new hashtable structure with the entries.
 */
struct hash_table * hashtable_grow(struct hash_table *ht);

/**
 * @brief Check that there is enough capacity in the hashtable to add more entries.
 * @param ht Hashtable to be checked
 * @return Returns 1 if there is > 10% capacity available; otherwise, returns 0
 */
int hashtable_check(struct hash_table *ht);

/**
 * @brief Move the items from the old hashtable to the new one
 * @param oldHt Previous hash table
 * @param nextHt Next hash table
 * @return 1 if successful; otherwise, returns 0
 */
int hashtable_move(struct hash_table *oldHt, struct hash_table *nextHt);


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

#endif //GRAPHDATA_HASHTABLE_H
