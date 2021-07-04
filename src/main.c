#include <passgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static passgen_config_t const default_passgen_config = {
    .count = 16,
    .extended = true,
    .passgen = PASSGEN_RANDOM,
    .outlen = OUTLEN_RANDOM,
};

static inline void parse_args(passgen_config_t *config, int argc, char const *argv[]) {
}

int main(int argc, char const *argv[]) {
	passgen_config_t config = default_passgen_config;
	parse_args(&config, argc, argv);

    size_t len = config.outlen(config.count);
    
    size_t res;
    char out[len + 1];
    if (len != (res = passgen(&config, out, len))) {
        fprintf(stderr, "error: expected %zu chars, found %zu\n", len, res);
        return EXIT_FAILURE;
    }
    
    out[len] = 0;
    fprintf(stdout, "%s\n", out);
	return EXIT_SUCCESS;
}
