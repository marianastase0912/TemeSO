#ifndef __HASHMAP_H_
#define __HASHMAP_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZEKEY 256
#define SIZEVALUE 256
#define N 131

typedef struct HashMapElement {
	char key[SIZEKEY];
	char value[SIZEVALUE];
} HashMapElement;

typedef struct HashMap {
	HashMapElement *hashmap;
} HashMap;

HashMap *initMap(void);
int hash_function(char *key);
void insert(HashMap *map, char *key, char *value);
void delete(HashMap *map, char *key);
char *findInMap(HashMap *map, char *key);
char *getValue(HashMap *map, char *key);
char *getKey(HashMap *map, int index);
void freeMem(HashMap *hash);

#endif
