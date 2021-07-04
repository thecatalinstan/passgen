#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <passgen.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

static char const *stdpool = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ0123456789";
static char const *extpool = "!@#$%^&*()-_=+[]{};:'\"\\|,<.>/?`~";

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

static inline void reshuffle_pool(char *pool, size_t poollen) {
    for (size_t i = 0; i < poollen; i++) {
        strshuffle(pool);
    }
}

#define ngroups 6
static size_t const groups[ngroups] = {4, 3, 4, 4, 3, 4};

typedef size_t (* outlen_func_t)(size_t);
typedef size_t (* passgen_func_t)(char *, const size_t, char *, const size_t);

typedef struct {
    size_t count;
    bool extended;
    passgen_func_t passgen;
    outlen_func_t outlen;
} passgen_config_t;

static inline size_t outlen_random(size_t count) {
    return count;
}

static inline size_t passgen_random(char *out, const size_t count, char *pool, const size_t poollen) {
    size_t offset = 0;
    do {
        reshuffle_pool(pool, poollen);
        size_t num = MIN(poollen, count - offset);
        memcpy(out + offset, pool, num);
        offset += num;
    } while (offset < count);
    
    return offset;
}

static inline size_t outlen_groups(size_t count) {
    size_t res = 0;
    res = count - 1;
    for (size_t i = 0; i < count; i++) {
        res += groups[i % ngroups];
    }
    return res;
}

static inline size_t passgen_groups(char *out, const size_t count, char *pool, const size_t poollen) {
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

static passgen_config_t const default_passgen_config = {
    .count = 16,
    .extended = false,
    .passgen = (passgen_func_t)passgen_random,
    .outlen = (outlen_func_t)outlen_random,
};

static inline void parse_args(passgen_config_t *config, int argc, char const *argv[]) {
    config->passgen = passgen_groups;
    config->outlen = outlen_groups;
    config->extended = true;
}

int main(int argc, char const *argv[]) {
	passgen_config_t config = default_passgen_config;
	parse_args(&config, argc, argv);

	size_t stdpoollength = strlen(stdpool);
	size_t extpoollength = strlen(extpool);

	size_t poollength = stdpoollength;
	if (config.extended) {
		poollength += extpoollength;
	}

	char pool[poollength + 1];
	pool[poollength] = 0;
	memcpy(pool, stdpool, poollength);
	if (config.extended) {
		memcpy(pool + stdpoollength, extpool, extpoollength);
	}
    
	size_t len = config.outlen(config.count);
	char out[len + 1];	
	out[len] = 0;
    
    size_t res;
    if (len != (res = config.passgen(out, config.count, pool, poollength))) {
        fprintf(stderr, "error: expected %zu chars, found %zu\n", len, res);
        return EXIT_FAILURE;
    }
    
    fprintf(stdout, "%s\n", out);
	return EXIT_SUCCESS;
}
