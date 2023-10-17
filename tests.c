#include "hashtable.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// tests

int htCreateDestroy() {
    hashtable* t = hashtableCreate(sizeof(int), 14, 0);
    if (t->elementLength != 14) {
        printf("Failed test. hashtable.elementLength not correct expected %d. "
               "Got %d",
               14, t->elementLength);
        return 0;
    }
    if (t->elementStride != sizeof(int)) {
        printf("Failed test. hashtable.elementStride not correct expected %lu. "
               "Got %d",
               sizeof(int), t->elementStride);
        return 0;
    }
    if (t->memory == 0) {
        printf("Failed test. hashtable.memory equals zero. Not malloced "
               "correctly.");
        return 0;
    }
    hashtableDestroy(t);
    return 1;
}

int htInt() {
    hashtable* t = hashtableCreate(sizeof(int), 4, 1);
    int v = 22;
    hashtableSet(t, "Int", &v);
    int x;
    hashtableGet(t, "Int", &x);
    if (x != v) {
        printf("Failed test. expected: %d. Got %d\n", v, x);
        return 0;
    }
    return 1;
}

typedef struct test2 {
    int num2;
} test2;

typedef struct testStruct {
    int num;
    char letter[50];
    float pi;
    test2* myTest;
} testStruct;

int htStruct() {
    testStruct ts;
    ts.num = 14;
    ts.pi = 3.14f;
    strcpy(ts.letter, "This is actually a string");
    test2 mt;
    mt.num2 = 35;
    ts.myTest = &mt;

    hashtable* t = hashtableCreate(sizeof(testStruct), 4, 1);
    hashtableSet(t, "my table", &ts);
    testStruct* x = malloc(sizeof(testStruct));
    hashtableGet(t, "my table", x);
    if (x->num != 14) {
        printf("Failed test num. Setting and Getting a struct pointer failed. "
               "Expected %d. Got %d\n",
               14, x->num);
        return 0;
    }
    if (fabsf(x->pi - 3.14f) > .0000000001f) {
        printf("Failed test pi. Setting and Getting a struct pointer failed. "
               "Expected %f. Got %f\n",
               3.14f, x->pi);
        return 0;
    }
    if (strcmp(x->letter, "This is actually a string")) {
        printf("Failed test letter. Setting and Getting a struct pointer "
               "failed. Expected %s. Got %s\n",
               "This is actually a string", x->letter);
        return 0;
    }

    testStruct t2;
    t2.num = 38;
    t2.pi = 3.142;
    strcpy(t2.letter, "C");
    test2 mt2;
    mt2.num2 = 11;
    t2.myTest = &mt2;
    hashtableSet(t, "other table", &t2);
    testStruct* s = malloc(sizeof(testStruct));
    hashtableGet(t, "other table", s);

    if (s->num != 38) {
        printf("Failed test num. Setting and Getting a struct pointer failed. "
               "Expected %d. Got %d\n",
               38, s->num);
        return 0;
    }
    if (fabsf(s->pi - 3.142f) > 0.001f) {
        printf("Failed test. Setting and Getting a struct pointer failed. "
               "Expected %f. Got %f\n",
               3.142f, s->pi);
        return 0;
    }
    if (strcmp(s->letter, "C")) {
        printf("Failed test other table. Setting and Getting a struct pointer "
               "failed. Expected %s. Got %s\n",
               "C", s->letter);
        return 0;
    }
    return 1;
}

int main() {
    printf("Starting tests...\n");
    int x = htCreateDestroy();
    int y = htInt();
    int z = htStruct();
    return 0;
}
