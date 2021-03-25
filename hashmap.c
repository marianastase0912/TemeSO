#include "hashmap.h"

// initialize the hash map structure and check for malloc errors
HashMap *initMap(void)
{
	int i;
	HashMap *hash = (HashMap *)malloc(sizeof(struct HashMap));

	if (hash == NULL)
		return NULL;
	hash->hashmap = (HashMapElement *)malloc(N *
			sizeof(struct HashMapElement));
	if (hash->hashmap == NULL) {
		free(hash);
		return NULL;
	}
	for (i = 0; i < N; i++) {
		memset(hash->hashmap[i].key, 0, SIZEKEY);
		memset(hash->hashmap[i].value, 0, SIZEVALUE);
	}
	return hash;
}

// hashing function for unique placement
int hash_function(char *key)
{
	int hash = 0;
	int ch;

	while ((ch = *key++))
		hash = hash + ch;

	if (hash < 0)
		hash = hash * (-1);

	return hash % N;
}

/*
 * insert function, insert the key and value at the hashIndex,
 * if the key exists, update value
 */
void insert(HashMap *map, char *key, char *value)
{
	int hashIndex = hash_function(key);
	int checkKey = strncmp(map->hashmap[hashIndex].key, key, SIZEKEY);
	int checkValue = strncmp(map->hashmap[hashIndex].value,
			value, SIZEVALUE);

	if (checkKey == 0 && checkValue == 0) {
		strncpy(map->hashmap[hashIndex].value, value, strlen(value));
	} else {
		strncpy(map->hashmap[hashIndex].key, key, strlen(key));
		strncpy(map->hashmap[hashIndex].value, value, strlen(value));
	}
}

// function to delete an entry from map
void delete(HashMap *map, char *key)
{
	int hashIndex = 0;

	hashIndex = hash_function(key);

	if (findInMap(map, key) != NULL) {
		memset(map->hashmap[hashIndex].key, 0, SIZEKEY);
		memset(map->hashmap[hashIndex].value, 0, SIZEVALUE);
	}
}

// function to find an entry by key
char *findInMap(HashMap *map, char *key)
{
	int hashIndex = 0;
	int checkKey = -1;

	hashIndex = hash_function(key);
	checkKey = strncmp(getKey(map, hashIndex), key, SIZEKEY);

	if (checkKey != 0)
		return NULL;

	return map->hashmap[hash_function(key)].value;
}

// function to return the value after key
char *getValue(HashMap *map, char *key)
{
	int hashIndex = 0;
	int checkKey = -1;

	hashIndex = hash_function(key);
	checkKey = strncmp(getKey(map, hashIndex), key, SIZEKEY);

	if (checkKey != 0)
		return NULL;

	return map->hashmap[hashIndex].value;
}

// function to return the key after the index in the map
char *getKey(HashMap *map, int index)
{
	return map->hashmap[index].key;
}

// free memory from the map
void freeMem(HashMap *hash)
{
	free(hash->hashmap);
	free(hash);
}
