#ifndef DS_COMMON_H
#define DS_COMMON_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    DS_SUCCESS = 0,
    DS_ERR_NULL_PTR = -1,
    DS_ERR_OUT_OF_MEMORY = -2,
    DS_ERR_EMPTY = -3,
    DS_ERR_OUT_OF_BOUNDS = -4,
    DS_ERR_NOT_FOUND = -5,
    DS_ERR_DUPLICATE = -6
} ds_status_t;

#endif /* DS_COMMON_H */
