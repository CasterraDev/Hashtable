#pragma once

typedef struct hashtable{
    int elementStride;
    int elementLength;
    char isPointerData;
    void* memory;
} hashtable;

/**
 * @brief Creates a hashtable and mallocs the needed memory
 * @param elementStride the stride of the data stored
 * @param elementLength the total length of the hashtable. (Maximum amount of items) CANNOT be resized
 * @param isPointerData will this hashtable be filled with pointers
 * @param outHashtable the hashtable that is given to you. Which you can use in the other functions 
 */
hashtable* hashtableCreate(int elementStride, int elementCnt, char isPointerData);

/**
 * @brief Fills the entire hashtable with param value. Will overwrite.
 * @param *ht pointer to the hashtable
 * @param *value the value you want the hashtable to be filled with
 */
void hashtableFill(hashtable *ht, void *value);

/**
 * @brief Gets the value of key from the hashtable
 * @param *ht pointer to the hashtable
 * @param *key key string to the value
 * @param *outValue the pointer to the value
 */
void hashtableGet(hashtable *ht, const char *key, void* outValue);

/**
 * @brief Sets the value of key in the given hashtable
 * @param *ht pointer to the hashtable
 * @param *key key string to the value you want to Sets
 * @param *value value you want to set key to
 */
char hashtableSet(hashtable *ht, const char *key, void *value);

/**
 * @brief Destroy the given hashtable. Frees all memory
 * @param *ht pointer to the hashtable
 */
void hashtableDestroy(hashtable* ht);
