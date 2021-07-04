#include "passgen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "shuffle.h"

__attribute__((constructor))
static void init(void) {
    srand((unsigned)(time(NULL) * 201906270400  + 19810710134500));
}

size_t passgen(const passgen_config_t * restrict config, char * restrict res, const size_t len) {
    static char const *stdpool = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ0123456789";
    static char const *extpool = "!@#$%^&*()-_=+[]{};:'\"\\|,<.>/?`~";

    size_t stdpoollength = strlen(stdpool);
    size_t extpoollength = strlen(extpool);

    size_t poollength = stdpoollength;
    if (config->extended) {
        poollength += extpoollength;
    }

    char pool[poollength + 1];
    pool[poollength] = 0;
    memcpy(pool, stdpool, poollength);
    if (config->extended) {
        memcpy(pool + stdpoollength, extpool, extpoollength);
    }
    
    size_t outlen = config->outlen(config->count);
    char out[outlen + 1];
    out[outlen] = 0;
    
    size_t r;
    if (outlen == (r = config->passgen(out, config->count, pool, poollength))) {
        r = MIN(outlen, len);
        memcpy(res, out, r);
    }
    
    return r;
}
