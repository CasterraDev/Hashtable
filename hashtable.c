#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_KEY "INVALID_KEY"

typedef struct entry {
  char *key;
  void *value;
} entry;

int hash(const char *key, int elementLength) {
  int primeCnt = 26;
  const unsigned int primes[] = {
      53,        97,        193,       389,       769,       1543,     3079,
      6151,      12289,     24593,     49157,     98317,     196613,   393241,
      786433,    1572869,   3145739,   6291469,   12582917,  25165843, 50331653,
      100663319, 201326611, 402653189, 805306457, 1610612741};

  const unsigned char *x;

  int hash = 0;
  for (x = (const unsigned char *)key; *x; x++) {
    int y = (*x % 26);
    unsigned int test = primes[y];
    hash = hash * primes[y] + *x;
  }

  hash = hash % elementLength;

  return hash;
}

void hashtableFillEntry(hashtable *ht, entry *value);

void hashtableCreate(int elementStride, int elementLength,
                     hashtable *outHashtable) {
  void *memory = malloc(elementLength * (elementStride + sizeof(entry)));
  memset(memory, 0, elementStride * elementLength);
  outHashtable->elementStride = elementStride;
  outHashtable->elementLength = elementLength;
  outHashtable->memory = memory;
  entry e;
  e.key = INVALID_KEY;
  hashtableFillEntry(outHashtable, &e);
  return;
}

void hashtableDestroy(hashtable *ht) {
  for (int i = 0; i < ht->elementLength; i++) {
    entry *e = (entry *)ht->memory + (ht->elementStride * i);
    free(e->key);
    if (e->value) {
      free(e->value);
    }
  }

  free(ht->memory);
}

char hashtableSet(hashtable *ht, const char *key, void *value) {
  int h = hash(key, ht->elementLength);
  int y = h;
  for (int i = 0; i < ht->elementLength; i++) {
    entry *e = (entry *)ht->memory + (ht->elementStride * y);

    if (strcmp(e->key, key) || strcmp(e->key, INVALID_KEY)) {
      entry t;
      t.key = key;
      t.value = value;
      memcpy(e, &t, ht->elementStride + sizeof(entry));
      return 1;
    }
    y++;
    y = y % ht->elementLength;
  }
  printf("ERROR: Could not find key entry or a free slot for key: %s", key);
  return 0;
}

void *hashtableGet(hashtable *ht, const char *key) {
  int h = hash(key, ht->elementLength);
  int y = h;
  for (int i = h; i < ht->elementLength; i++) {
    entry *e = (entry *)ht->memory + (ht->elementStride * y);
    if (strcmp(e->key, INVALID_KEY)) {
      return NULL;
    }
    if (strcmp(e->key, key)) {
      return e->value;
    }
    y++;
    y = y % ht->elementLength;
  }
  return NULL;
}

void hashtableFill(hashtable *ht, void *value) {
  for (int i = 0; i < ht->elementLength; i++) {
    entry *e = (entry *)ht->memory + (ht->elementStride * ht->elementLength);
    memcpy(e->value, value, sizeof(ht->elementStride));
  }
}
// "Secret" function to make all the slots invalid when ht is created
void hashtableFillEntry(hashtable *ht, entry *value) {
  for (int i = 0; i < ht->elementLength; i++) {
    entry *e = (entry *)ht->memory + (ht->elementStride * ht->elementLength);
    memcpy(e, value, sizeof(entry));
  }
}
