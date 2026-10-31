#include "bloom_filter.h"
#include <stdlib.h>
#include <string.h>

#define BITS_PER_WORD 64

/* 64-bit FNV-1a Hash */
static uint64_t fnv1a_hash(const void *data, size_t len) {
    const unsigned char *bytes = (const unsigned char *)data;
    uint64_t hash = 14695981039346656037ULL;
    for (size_t i = 0; i < len; ++i) {
        hash ^= (uint64_t)bytes[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

/* 64-bit MurmurHash3 mix */
static uint64_t murmur64_mix(uint64_t k) {
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    return k;
}

static void compute_two_hashes(const void *data, size_t len, uint64_t *h1, uint64_t *h2) {
    uint64_t h = fnv1a_hash(data, len);
    *h1 = h;
    *h2 = murmur64_mix(h);
    if (*h2 == 0) {
        *h2 = 1;
    }
}

ds_status_t bloom_create(size_t num_bits, size_t num_hashes, bloom_filter_t **out_filter) {
    if (!out_filter || num_bits == 0 || num_hashes == 0) {
        return DS_ERR_INVALID_ARG;
    }

    bloom_filter_t *filter = (bloom_filter_t *)malloc(sizeof(bloom_filter_t));
    if (!filter) {
        return DS_ERR_OUT_OF_MEMORY;
    }

    size_t words = (num_bits + BITS_PER_WORD - 1) / BITS_PER_WORD;
    filter->bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!filter->bits) {
        free(filter);
        return DS_ERR_OUT_OF_MEMORY;
    }

    filter->num_bits = num_bits;
    filter->num_hashes = num_hashes;
    *out_filter = filter;
    return DS_SUCCESS;
}

void bloom_destroy(bloom_filter_t *filter) {
    if (!filter) return;
    free(filter->bits);
    free(filter);
}

ds_status_t bloom_add(bloom_filter_t *filter, const void *data, size_t len) {
    if (!filter || !data) return DS_ERR_INVALID_ARG;

    uint64_t h1, h2;
    compute_two_hashes(data, len, &h1, &h2);

    for (size_t i = 0; i < filter->num_hashes; ++i) {
        uint64_t bit_idx = (h1 + i * h2) % filter->num_bits;
        size_t word_idx = bit_idx / BITS_PER_WORD;
        size_t bit_pos = bit_idx % BITS_PER_WORD;
        filter->bits[word_idx] |= (1ULL << bit_pos);
    }
    return DS_SUCCESS;
}

bool bloom_contains(const bloom_filter_t *filter, const void *data, size_t len) {
    if (!filter || !data) return false;

    uint64_t h1, h2;
    compute_two_hashes(data, len, &h1, &h2);

    for (size_t i = 0; i < filter->num_hashes; ++i) {
        uint64_t bit_idx = (h1 + i * h2) % filter->num_bits;
        size_t word_idx = bit_idx / BITS_PER_WORD;
        size_t bit_pos = bit_idx % BITS_PER_WORD;
        if (!(filter->bits[word_idx] & (1ULL << bit_pos))) {
            return false;
        }
    }
    return true;
}

void bloom_clear(bloom_filter_t *filter) {
    if (!filter || !filter->bits) return;
    size_t words = (filter->num_bits + BITS_PER_WORD - 1) / BITS_PER_WORD;
    memset(filter->bits, 0, words * sizeof(uint64_t));
}
