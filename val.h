#pragma once

#include <stdbool.h>

typedef enum {
    TYPE_INT,
    TYPE_STR,
} val_type_t;

typedef struct {
    val_type_t type;
    union {
        char *string;
        int integer;
    } as;
} val_t;

bool val_eq_compare_bytes(val_t *a, val_t *b);
