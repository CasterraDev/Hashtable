#pragma once

typedef struct hashtable{
    int elementStride;
    int elementLength;
    void* memory;
} hashtable;

void hashtableCreate(int elementStride, int elementCnt, hashtable* outHashtable);

void hashtableFill(hashtable *ht, void *value);

void *hashtableGet(hashtable *ht, const char *key);

char hashtableSet(hashtable *ht, const char *key, void *value);

void hashtableDestroy(hashtable* ht);
