#include <stdlib.h>
#include <printf.h>
#include "linked-lists.h"
#include "map.h"
#include "val.h"

static map_buckets_t *buckets_new(uint32_t nbuckets) {
    map_buckets_t *buckets = (map_buckets_t *) malloc(sizeof(map_buckets_t) * nbuckets);
    for (uint32_t i = 0; i < nbuckets; i++) {
        buckets->nodes[i] = NULL;
    }
    buckets->nelems = 0;
    buckets->nbuckets = nbuckets;
    return buckets;
}

map_collisions_linked_t *map_collisions_linked_new(
    uint8_t nbuckets,
    uint32_t (*hash_func)(val_t *node),
    bool (*eq_func)(val_t *node, val_t *other)
) {
    map_buckets_t *buckets = buckets_new(nbuckets);

    map_collisions_linked_t *map = (map_collisions_linked_t *) malloc(sizeof(map_collisions_linked_t));

    if (buckets == NULL || map == NULL) {
        printf("Out of memory.\n");
        exit(1);
    }

    map->hash_func = hash_func;
    map->eq_func = eq_func;
    map->buckets = buckets;

    return map;
}

void map_collisions_linked_free(map_collisions_linked_t *map) {
    free(map->buckets);
    free(map);
}

static void buckets_put_internal(map_buckets_t *buckets, val_t *k, val_t *v,
                                 uint32_t (*hash_func)(val_t *val),
                                 bool (*eq_func)(val_t *a, val_t *b)) {

    if (buckets == NULL) {
        printf("why null?\n");
        exit(1);
    }

    uint32_t hash_val = hash_func(k);
    uint32_t bucket_index = hash_val % buckets->nbuckets;

    map_kv_node_t *first = (map_kv_node_t *) buckets->nodes[bucket_index];
    map_kv_node_t *node = first;
    while (node != NULL) {
        if (node->hash_val == hash_val &&
            eq_func(node->k, k)) {
            // Key already in the map. Update the value.
            *(node->v) = *v;
            return;
        }
        node = node->next;
    }

    // Create a new node.
    map_kv_node_t *new = (map_kv_node_t *) malloc(sizeof(map_kv_node_t));
    if (new == NULL) {
        exit(1);
    }

    new->hash_val = hash_val;
    new->k = (val_t *) malloc(sizeof(val_t));
    *(new->k) = *k;
    new->v = (val_t *) malloc(sizeof(val_t));
    *(new->v) = *v;

    // Insert at head of list in this bucket.
    new->next = (first == NULL) ? NULL : first;
    buckets->nodes[bucket_index] = new;
    buckets->nelems++;
}

static void maybe_grow_map(map_collisions_linked_t *map) {
    uint32_t new_nbuckets =
        MAP_NEW_BUCKETS +
        (map->buckets->nelems / MAP_NEW_BUCKETS) * MAP_NEW_BUCKETS;

    if (new_nbuckets == map->buckets->nbuckets) {
        return;
    }

    map_buckets_t *new_buckets = buckets_new(new_nbuckets);

    printf("Resizing map to %d buckets...\n", new_nbuckets);

    for (uint32_t i = 0; i < map->buckets->nbuckets; i++) {
        map_kv_node_t *orig_kv = map->buckets->nodes[i];
        while (orig_kv != NULL) {
            buckets_put_internal(new_buckets, orig_kv->k, orig_kv->v, map->hash_func, map->eq_func);
            orig_kv = orig_kv->next;
        }
    }

    map_buckets_t *old_buckets = map->buckets;
    map->buckets = new_buckets;
    free(old_buckets);
}

void map_collisions_linked_put(map_collisions_linked_t *map, val_t *k, val_t *v) {
    maybe_grow_map(map);
    return buckets_put_internal(map->buckets, k, v, map->hash_func, map->eq_func);
}

val_t *map_collisions_linked_get(map_collisions_linked_t *map, val_t *k) {
    uint32_t hash_val = map->hash_func(k);
    uint32_t bucket_index = hash_val % map->buckets->nbuckets;

    map_kv_node_t *node = map->buckets->nodes[bucket_index];
    while (node != NULL) {
        if ((node->hash_val == hash_val) &&
            (map->eq_func(node->k, k))) {
            return node->v;
        }
        node = node->next;
    }

    return NULL;
}
