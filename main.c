#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "words.h"
#include "linked-lists.h"
#include "hash.h"
#include "val.h"

static double test_collisions_linked(void) {
    printf("Testing hash table with linked-lists for collisions.\n");
    double start = clock();

    map_collisions_linked_t *map = map_collisions_linked_new(MAP_NEW_BUCKETS,
                                                             &val_hash, &val_eq_compare_bytes);

    printf("Writing 1000 values\n");
    val_t v = {
        .type= TYPE_INT,
        .as.integer = 1,
    };
    for (int i = 0; i < 1000; i++) {
        val_t k = {
            .type = TYPE_STR,
            .as.string = *(words_a + i)
        };
        map_collisions_linked_put(map, &k, &v);
    }

    printf("Looking up 1000 extant values\n");
    for (int i = 0; i < 1000; i++) {
        val_t k = {
            .type = TYPE_STR,
            .as.string = *(words_a + i)
        };
        val_t *found = map_collisions_linked_get(map, &k);
        if (found->as.integer != 1) {
            printf("Key %s: Unexpected value %d\n", k.as.string, found->as.integer);
            exit(1);
        };
    }

    printf("Looking up 1000 non-extant values\n");
    for (int i = 0; i < 1000; i++) {
        val_t k = {
            .type = TYPE_STR,
            .as.string = *(words_z + i)
        };
        (void) map_collisions_linked_get(map, &k);
    }

    double end = clock();

    map_collisions_linked_free(map);
    return end - start;
}

int main(void) {
    double sum = 0;
    for (int i = 0; i < 10; i++) {
        printf("== Iteration %d of 10\n", i + 1);
        sum += test_collisions_linked();
    }
    double avg = sum / 10;
    printf("Map with linked lists for collisions: Avg run time %g ticks\n", avg);
}
