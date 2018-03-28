//
// Created by l.hajrullai on 14/03/2018.
//

#include "ex_commons.h"

void *Malloc(size_t n) {
    void *new = malloc(n);
    if (new == NULL) FATAL_ERROR("Out of memory.");
    return new;
}