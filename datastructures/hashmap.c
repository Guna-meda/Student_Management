#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static unsigned int hash(const char *key, unsigned int size) {
    unsigned long h = 5381;
    int c;
    while ((c = *key++)) h = ((h << 5) + h) + c;
    return h % size;
}

HashMap* hm_create(unsigned int size) {
    HashMap *m = malloc(sizeof(HashMap));
    if (!m) return NULL;
    m->size = size;
    m->buckets = calloc(size, sizeof(HashEntry*));
    return m;
}

static void free_entry(HashEntry *e, void (*free_value)(void*)) {
    if (e) {
        if (free_value && e->value) free_value(e->value);
        free(e);
    }
}

void hm_destroy(HashMap *map, void (*free_value)(void*)) {
    if (!map) return;
    for (unsigned i = 0; i < map->size; ++i) {
        HashEntry *e = map->buckets[i], *next;
        while (e) { next = e->next; free_entry(e, free_value); e = next; }
    }
    free(map->buckets);
    free(map);
}

void* hm_get(HashMap *map, const char *key) {
    unsigned idx = hash(key, map->size);
    for (HashEntry *e = map->buckets[idx]; e; e = e->next)
        if (strcmp(e->key, key) == 0) return e->value;
    return NULL;
}

void hm_put(HashMap *map, const char *key, void *value) {
    unsigned idx = hash(key, map->size);
    HashEntry *e = map->buckets[idx];
    while (e) {
        if (strcmp(e->key, key) == 0) { e->value = value; return; }
        e = e->next;
    }
    /* not found → prepend */
    HashEntry *ne = malloc(sizeof(HashEntry));
    strncpy(ne->key, key, sizeof(ne->key)-1); ne->key[sizeof(ne->key)-1] = '\0';
    ne->value = value;
    ne->next = map->buckets[idx];
    map->buckets[idx] = ne;
}

void* hm_remove(HashMap *map, const char *key) {
    unsigned idx = hash(key, map->size);
    HashEntry **pp = &map->buckets[idx], *e;
    while ((e = *pp)) {
        if (strcmp(e->key, key) == 0) {
            *pp = e->next;
            void *v = e->value;
            free(e);
            return v;
        }
        pp = &e->next;
    }
    return NULL;
}

void hm_foreach(HashMap *map, void (*action)(const char *key, void *value)) {
    for (unsigned i = 0; i < map->size; ++i)
        for (HashEntry *e = map->buckets[i]; e; e = e->next)
            action(e->key, e->value);
}