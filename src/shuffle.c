//
//  shuffle.c
//  passgen (lib)
//
//  Created by Cătălin Stan on 04/07/2021.
//

#include "shuffle.h"

#include <string.h>

static inline void strshuffle(char *str) {
    static const char marker = '\0';
    size_t length = strlen(str);

    char temp[length];
    memcpy(temp, str, length);
    for (size_t i = 0; i < length; i++) {
        size_t key;
        do {
            key = rand() % length;
        } while (temp[key] == marker);
        str[i] = temp[key];
        temp[key] = marker;
    }
}

void reshuffle_pool(char *pool, size_t poollen) {
    for (size_t i = 0; i < poollen; i++) {
        strshuffle(pool);
    }
}
