//
//  passgen_random.c
//  passgen (lib)
//
//  Created by Cătălin Stan on 04/07/2021.
//

#include <passgen.h>

#include <stdlib.h>
#include <string.h>

#include "shuffle.h"

size_t outlen_random(size_t count) {
    return count;
}

size_t passgen_random(char *out, const size_t count, char *pool, const size_t poollen) {
    size_t offset = 0;
    do {
        reshuffle_pool(pool, poollen);
        size_t num = MIN(poollen, count - offset);
        memcpy(out + offset, pool, num);
        offset += num;
    } while (offset < count);
    
    return offset;
}



