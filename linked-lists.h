#pragma once

#include <stdint.h>
#include "map.h"
#include "val.h"

typedef struct map_kv_node_t {
    val_t *k;
    val_t *v;
    struct map_kv_node_t *next;
    uint32_t hash_val;
} map_kv_node_t;

typedef struct {
    uint32_t nbuckets;
    uint32_t nelems;
    // Allocated to size when struct is instantiated;
    map_kv_node_t *nodes[1];
} map_buckets_t;

typedef struct {
    map_buckets_t *buckets;

    uint32_t (*hash_func)(val_t *val);

    bool (*eq_func)(val_t *a, val_t *b);
} map_collisions_linked_t;

map_collisions_linked_t *map_collisions_linked_new(
    uint8_t nbuckets,
    uint32_t (*hash_func)(val_t *node),
    bool (*eq_func)(val_t *node, val_t *other)
);

void map_collisions_linked_free(map_collisions_linked_t *map);

void map_collisions_linked_put(map_collisions_linked_t *map, val_t *k, val_t *v);

val_t *map_collisions_linked_get(map_collisions_linked_t *map, val_t *k);
