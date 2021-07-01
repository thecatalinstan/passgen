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

typedef enum {
	passgen_mode_none 	= 0,
	passgen_mode_groups,
	passgen_mode_random,

	passgen_mode_max,
} passgen_mode_t;

typedef struct {
	passgen_mode_t mode;
	bool extended;
	int outlen;
} passgen_config_t;

static passgen_config_t const default_passgen_config = {
	.mode = passgen_mode_random,
	.extended = false,
	.outlen = 100,
};

#define ngroups 6
static int const groups[ngroups] = {4, 3, 4, 4, 3, 4};
static char const separator = '-';

static inline void shuffle(char *str) {
	static const char marker = '\0';
	int length = strlen(str);

	char temp[length];
	strcpy(temp, str);
	for (int i = 0; i < length; i++) {            
		int key;
		do {
			key = rand() % length;
			// key = arc4random_uniform(length);
		} while (temp[key] == marker);
		str[i] = temp[key];
		temp[key] = marker;		
	}

}

static inline void passgen_random(char *out, const int outlen, const char *shuffled) {
	memcpy(out, shuffled, outlen);
}

static inline void passgen_groups(char *out, const int outlen, const char *shuffled) {
	int idx = 0;
	for (int i = 0; i < ngroups; i++) {
		int len = groups[i];
		memcpy(out + idx, shuffled + idx, len);
		idx += len;
		if (i == ngroups - 1) {
			continue;
		}
		out[idx] = separator;
		idx += 1;
	}
}

static inline int passgen(char *out, const int outlen, const char *in, passgen_mode_t mode) {
	int length = strlen(in);	
	char shuffled[length + 1];
	strcpy(shuffled, in);
	for (int i = 0; i < length; ++i) {		
		shuffle(shuffled);
	}

	switch(mode) {
		case passgen_mode_groups:
		passgen_groups(out, outlen, shuffled);
		break;

		case passgen_mode_random:
		passgen_random(out, outlen, shuffled);
		break;

		default:
		break;
	}

	out[outlen] = '\0';

	return strlen(out);
}

static inline void parse_args(passgen_config_t *config, int argc, char const *argv[]) {
	if (config->mode <= passgen_mode_none || config->mode >= passgen_mode_max) {
		config->mode = default_passgen_config.mode;
	}

	if(config->mode == passgen_mode_groups) {
		config->extended = false;
		config->outlen = ngroups - 1;	
		for (int i = 0; i < ngroups; i++) {
			config->outlen += groups[i];
		}	
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

	char in[poollength + 1];
	in[poollength] = 0;
	memcpy(in, stdpool, poollength);
	if (config.extended) {
		memcpy(in + stdpoollength, extpool, extpoollength);
	}	

	srand(time(NULL) * poollength + 119810710134500);

	char out[config.outlen + 1];	
	out[config.outlen] = 0;
	int total = 0, generated = 0;
	do {
		char temp[poollength + 1];
		generated = passgen(temp, poollength, in, config.mode);
		int count = MIN(generated, config.outlen - total);
		memcpy(out + total, temp, count);
		total += count;
	} while (total < config.outlen);
	
	if (config.outlen != total) {
		fprintf(stderr, "error: expected %d chars, found %d\n", config.outlen, (int)strlen(out));
	}

	fprintf(stderr, "%s\n", out);
	return 0;
}
