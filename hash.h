#pragma once

#include <stdint.h>
#include "val.h"

// FNV (Fowler, Noll, Vo) FNV-1a 32-bit hash constants.
#define FNV32Prime 0x01000193
#define FNV32Basis 0x811C9DC5

uint32_t val_hash(val_t *val);
