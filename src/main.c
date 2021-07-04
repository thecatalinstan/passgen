#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <passgen.h>

static char const *stdpool = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ0123456789";
static char const *extpool = "!@#$%^&*()-_=+[]{};:'\"\\|,<.>/?`~";

typedef size_t (* outlen_func_t)(size_t);
typedef size_t (* passgen_func_t)(char *, const size_t, char *, const size_t);

typedef struct {
    size_t count;
    bool extended;
    passgen_func_t passgen;
    outlen_func_t outlen;
} passgen_config_t;

static passgen_config_t const default_passgen_config = {
    .count = 16,
    .extended = false,
    .passgen = PASSGEN_RANDOM,
    .outlen = OUTLEN_RANDOM,
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
