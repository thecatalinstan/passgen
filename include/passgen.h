#ifndef _passgen_h_
#define _passgen_h_

#include <stdlib.h>

extern size_t outlen_groups(size_t count);
#define OUTLEN_GROUPS (outlen_func_t)outlen_groups

extern size_t passgen_groups(char *out, const size_t count, char *pool, const size_t poollen);
#define PASSGEN_GROUPS (passgen_func_t)passgen_groups

extern size_t outlen_random(size_t count);
#define OUTLEN_RANDOM (outlen_func_t)outlen_random

extern size_t passgen_random(char *out, const size_t count, char *pool, const size_t poollen);
#define PASSGEN_RANDOM (passgen_func_t)passgen_random

#endif
