#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_KEY "INVALID_KEY"

typedef struct entry {
    char key[50];
    void* value;
} entry;

int hash(const char* key, int elementLength) {
    int primeCnt = 26;
    const unsigned int primes[] = {
        53,        97,        193,      389,       769,       1543,
        3079,      6151,      12289,    24593,     49157,     98317,
        196613,    393241,    786433,   1572869,   3145739,   6291469,
        12582917,  25165843,  50331653, 100663319, 201326611, 402653189,
        805306457, 1610612741};

    const unsigned char* x;

    int hash = 0;
    for (x = (const unsigned char*)key; *x; x++) {
        int y = (*x % 26);
        hash = hash * primes[y] + *x;
    }

    hash = hash % elementLength;

    return hash;
}

void hashtableFillEntry(hashtable* ht, entry* value);

hashtable* hashtableCreate(int elementStride, int elementLength,
                           char isPointerData) {
    hashtable* outHashtable = malloc(sizeof(hashtable));
    void* memory = malloc(elementLength * (elementStride + sizeof(entry)));
    memset(outHashtable, 0, sizeof(hashtable));
    outHashtable->elementStride = elementStride;
    outHashtable->elementLength = elementLength;
    outHashtable->isPointerData = isPointerData;
    outHashtable->memory = memory;
    memset(outHashtable->memory, 0,
           (elementStride + sizeof(entry)) * elementLength);
    entry e;
    strcpy(e.key, INVALID_KEY);
    e.value = 0;
    hashtableFillEntry(outHashtable, &e);
    return outHashtable;
}

void hashtableDestroy(hashtable* ht) {
    free(ht->memory);
    free(ht);
}

char hashtableSet(hashtable* ht, const char* key, void* value) {
    int h = hash(key, ht->elementLength);
    int y = h;
    for (int i = 0; i < ht->elementLength; i++) {
        entry* e = (entry*)(ht->memory + (sizeof(entry) * y));
        if (!strcmp(e->key, key) || !strcmp(e->key, INVALID_KEY)) {
            entry t;
            strcpy(t.key, key);
            t.value = value;
            memcpy(e, &t, sizeof(entry));
            return 1;
        }
        y++;
        y = y % ht->elementLength;
    }
    printf("ERROR: Could not find key entry or a free slot for key: %s", key);
    return 0;
}

void hashtableGet(hashtable* ht, const char* key, void* outValue) {
    int h = hash(key, ht->elementLength);
    int y = h;
    for (int i = 0; i < ht->elementLength; i++) {
        entry* e = (entry*)(ht->memory + (sizeof(entry) * y));
        if (!strcmp(e->key, INVALID_KEY)) {
            return;
        }
        if (!strcmp(e->key, key)) {
            if (!ht->isPointerData) {
                memcpy(outValue, (void*)e->value, ht->elementStride);
            } else {
                memcpy(outValue, e->value, ht->elementStride);
            }
            return;
        }
        y++;
        y = y % ht->elementLength;
    }
    return;
}

void hashtableFill(hashtable* ht, void* value) {
    for (int i = 0; i < ht->elementLength; i++) {
        entry* e = (entry*)(ht->memory + (sizeof(entry) * i));
        memcpy(e->value, value, sizeof(ht->elementStride));
    }
}
// "Secret" function to make all the slots invalid when ht is created
void hashtableFillEntry(hashtable* ht, entry* value) {
    for (int i = 0; i < ht->elementLength; i++) {
        entry* e = (entry*)(ht->memory + (sizeof(entry) * i));
        memcpy(e, value, sizeof(entry));
    }
}
