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
static int const groups[ngroups] = {4, 3, 4, 4, 3, 4};

typedef enum {
	passgen_mode_none 	= 0,
	passgen_mode_groups,
	passgen_mode_random,

	passgen_mode_max,
} passgen_mode_t;

typedef struct {
	passgen_mode_t mode;
	int count;
	bool extended;
} passgen_config_t;

static passgen_config_t const default_passgen_config = {
	.mode = passgen_mode_random,
	.extended = false,
	.count = 124,
};

static inline int outlen(passgen_mode_t mode, int count) {
	int res = 0;
	switch (mode) {
		case passgen_mode_random:
		res = count;
		break;

		case passgen_mode_groups: {
			res = count - 1;
			for (int i = 0; i < count; i++) {
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
	int length = strlen(str);

	char temp[length];
	strcpy(temp, str);
	for (int i = 0; i < length; i++) {            
		int key;
		do {
			key = rand() % length;
		} while (temp[key] == marker);
		str[i] = temp[key];
		temp[key] = marker;		
	}
}

static inline int passgen_random(char *out, const int count, char *pool, const int poollen) {
    int total = 0;
    do {
        for (int i = 0; i < poollen; i++) {
            strshuffle(pool);
        }
        fprintf(stderr, " *** shf: %s\n", pool);
        int num = MIN(poollen, count - total);
        memcpy(out + total, pool, num);
        total += num;
    } while (total < count);
    
    return total;
}

//static inline int passgen_groups(char *out, const int len, const int count, const char *in) {
//    int length = strlen(in);
//    char shuffled[length + 1];
//    strcpy(shuffled, in);
//    for (int i = 0; i < length; ++i) {
//        strshuffle(shuffled);
//    }
//
//	static char const separator = '-';
//    int poollen = strlen(shuffled);
//
//	int idx = 0;
//	for (int i = 0; i < count; i++) {
//		int len = groups[i % ngroups];
//		memcpy(out + idx, shuffled + (idx % poollen), len);
//		idx += len;
//		if (i == count - 1) {
//			continue;
//		}
//		out[idx] = separator;
//		idx += 1;
//	}
//	out[idx] = '\0';
//}

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

	int stdpoollength = strlen(stdpool);	
	int extpoollength = strlen(extpool);	

	int poollength = stdpoollength;
	if (config.extended) {
		poollength += extpoollength;
	}

	char pool[poollength + 1];
	pool[poollength] = 0;
	memcpy(pool, stdpool, poollength);
	if (config.extended) {
		memcpy(pool + stdpoollength, extpool, extpoollength);
	}
    
	int len = outlen(config.mode, config.count);
	char out[len + 1];	
	out[len] = 0;
    
    int res = 0;
        
    switch(config.mode) {
        case passgen_mode_groups:
//            res = passgen_groups(out, config.count, in);
            break;
            
        case passgen_mode_random:
            res = passgen_random(out, config.count, pool, poollength);
            break;
            
        default:
            break;
    }

    if (res != len) {
        fprintf(stderr, "error: expected %d chars, found %d\n", len, res);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "%s\n", out);
	return EXIT_SUCCESS;
}

__attribute__((constructor))
void init(void) {
    srand(time(NULL) * 201906270400  + 19810710134500);
}
