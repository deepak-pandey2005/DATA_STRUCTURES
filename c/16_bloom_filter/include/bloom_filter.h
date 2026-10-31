#ifndef DS_BLOOM_FILTER_H
#define DS_BLOOM_FILTER_H

#include "common.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct bloom_filter {
    uint64_t *bits;
    size_t num_bits;
    size_t num_hashes;
} bloom_filter_t;

ds_status_t bloom_create(size_t num_bits, size_t num_hashes, bloom_filter_t **out_filter);
void bloom_destroy(bloom_filter_t *filter);

ds_status_t bloom_add(bloom_filter_t *filter, const void *data, size_t len);
bool bloom_contains(const bloom_filter_t *filter, const void *data, size_t len);
void bloom_clear(bloom_filter_t *filter);

#endif /* DS_BLOOM_FILTER_H */
