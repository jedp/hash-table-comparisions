#include <string.h>
#include <printf.h>
#include "val.h"

bool val_eq_compare_bytes(val_t *a, val_t *b) {
    if (a->type != b->type) {
        printf("different types.\n");
        return false;
    }
    switch (a->type) {
        case TYPE_INT:
            return (a->as.integer == b->as.integer);
        case TYPE_STR: {
            if (strlen(a->as.string) != strlen(b->as.string)) {
                return false;
            }
            return memcmp(a->as.string, b->as.string, strlen(a->as.string)) == 0;
        }
        default:
            printf("Unsupported type for primitive comparison: %d\n", a->type);
            return false;
    }
}
