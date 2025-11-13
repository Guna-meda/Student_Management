#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct HashEntry {
    char key[64];
    void *value;
    struct HashEntry *next;
} HashEntry;

typedef struct {
    HashEntry **buckets;
    unsigned int size;      /* number of buckets */
} HashMap;

HashMap* hm_create(unsigned int size);
void     hm_destroy(HashMap *map, void (*free_value)(void*));
void*    hm_get(HashMap *map, const char *key);
void     hm_put(HashMap *map, const char *key, void *value);
void*    hm_remove(HashMap *map, const char *key);
void     hm_foreach(HashMap *map, void (*action)(const char *key, void *value));

#endif