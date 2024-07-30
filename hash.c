#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "hash.h"
#include "val.h"

static uint32_t str_hash(const char *string) {
    uint32_t temp = 0;
    uint8_t b;

    /*
     * FNV (Fowler, Noll, Vo), the non-cryptographic hash function FNV-1a
     * for 32-bit hashes returning a 32-bit integer.
     */
    temp = FNV32Basis;
    size_t i = 0;
    while (string[i] != '\0') {
        b = string[i];
        temp = FNV32Prime * (temp ^ b);
        i++;
    }

    return temp;
}

uint32_t val_hash(val_t *val) {
    switch (val->type) {
        case TYPE_INT:
            return val->as.integer;
        case TYPE_STR:
            return str_hash(val->as.string);
        default:
            printf("We don't have that type yet\n");
            exit(1);
    }
}
