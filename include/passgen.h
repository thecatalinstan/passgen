#ifndef _passgen_h_
#define _passgen_h_

#include <stdbool.h>
#include <stdlib.h>

#if defined(__cplusplus)
#define PASSGEN_EXTERN extern "C"
#else
#define PASSGEN_EXTERN extern
#endif

#define PASSGEN_EXPORT PASSGEN_EXTERN

typedef size_t (* outlen_func_t)(size_t);
typedef size_t (* passgen_func_t)(char *, const size_t, char *, const size_t);

typedef struct {
    size_t count;
    bool extended;
    passgen_func_t passgen;
    outlen_func_t outlen;
} passgen_config_t;

PASSGEN_EXPORT size_t passgen(const passgen_config_t * restrict config, char * restrict pass, const size_t len);

PASSGEN_EXPORT size_t outlen_groups(size_t count);
#define OUTLEN_GROUPS (outlen_func_t)outlen_groups

PASSGEN_EXPORT size_t passgen_groups(char *pass, const size_t count, char *pool, const size_t poollen);
#define PASSGEN_GROUPS (passgen_func_t)passgen_groups

PASSGEN_EXPORT size_t outlen_random(size_t count);
#define OUTLEN_RANDOM (outlen_func_t)outlen_random

PASSGEN_EXPORT size_t passgen_random(char *pass, const size_t count, char *pool, const size_t poollen);
#define PASSGEN_RANDOM (passgen_func_t)passgen_random

#endif
