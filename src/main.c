#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

static char const *stdpool = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ0123456789";
static char const *extpool = "!@#$%^&*()-_=+[]{};:'\"\\|,<.>/?`~";

#define ngroups 6
static size_t const groups[ngroups] = {4, 3, 4, 4, 3, 4};

//size_t outlen(passgen_mode_t mode, size_t count)
//size_t passgen_random(char *out, const size_t count, char *pool, const size_t poollen) {
    
typedef enum {
	passgen_mode_none 	= 0,
	passgen_mode_groups,
	passgen_mode_random,

	passgen_mode_max,
} passgen_mode_t;

typedef struct {
	passgen_mode_t mode;
	size_t count;
	bool extended;
} passgen_config_t;

static passgen_config_t const default_passgen_config = {
	.mode = passgen_mode_groups,
	.extended = false,
	.count = ngroups,
};

static inline size_t outlen(passgen_mode_t mode, size_t count) {
	size_t res = 0;
	switch (mode) {
		case passgen_mode_random:
		res = count;
		break;

		case passgen_mode_groups: {
			res = count - 1;
			for (size_t i = 0; i < count; i++) {
				res += groups[i % ngroups];
			}
		}
		break;

		default:
		break;
	}
	
	return res;
}

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

static inline void parse_args(passgen_config_t *config, int argc, char const *argv[]) {
	if (config->mode <= passgen_mode_none || config->mode >= passgen_mode_max) {
		config->mode = default_passgen_config.mode;
	}

	if(config->mode == passgen_mode_groups) {
		config->extended = false;
	}
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
    
	size_t len = outlen(config.mode, config.count);
	char out[len + 1];	
	out[len] = 0;
    
    size_t res = 0;
        
    switch(config.mode) {
        case passgen_mode_groups:
            res = passgen_groups(out, config.count, pool, poollength);
            break;
            
        case passgen_mode_random:
            res = passgen_random(out, config.count, pool, poollength);
            break;
            
        default:
            break;
    }
    
    fprintf(stdout, "%s\n", out);

    if (res != len) {
        fprintf(stderr, "error: expected %zu chars, found %zu\n", len, res);
        return EXIT_FAILURE;
    }

//    fprintf(stdout, "%s\n", out);
	return EXIT_SUCCESS;
}

__attribute__((constructor))
void init(void) {
    srand((unsigned)(time(NULL) * 201906270400  + 19810710134500));
}
