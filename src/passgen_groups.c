//
//  passgen_groups.c
//  passgen (tool)
//
//  Created by Cătălin Stan on 04/07/2021.
//

#include <passgen.h>

#include <stdlib.h>
#include <string.h>

#include "shuffle.h"

#define ngroups 6
static size_t const groups[ngroups] = {4, 3, 4, 4, 3, 4};

size_t outlen_groups(size_t count) {
    size_t res = 0;
    res = count - 1;
    for (size_t i = 0; i < count; i++) {
        res += groups[i % ngroups];
    }
    return res;
}

size_t passgen_groups(char *out, const size_t count, char *pool, const size_t poollen) {
    static char const separator = '-';
    
    reshuffle_pool(pool, poollen);

    size_t offset = 0, pooloffset = 0;
    for (size_t i = 0; i < count; i++) {
        size_t grouplen = groups[i % ngroups];
        if (pooloffset + grouplen > poollen) {
            reshuffle_pool(pool, poollen);
            pooloffset = 0;
        }
        
        memcpy(out + offset, pool + pooloffset, grouplen);
        
        pooloffset += grouplen;
        offset += grouplen;
        
        if (i == count - 1) {
            continue;
        }
        
        out[offset] = separator;
        offset += 1;
    }
    
    return offset;
}
