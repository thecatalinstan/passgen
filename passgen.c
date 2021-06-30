#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
	passgen_mode_groups = 0,
	passgen_mode_random = 1,
} passgen_mode_t;

static const passgen_mode_t default_passgen_mode = passgen_mode_groups;

static char const *pool = "abcdefghijklmnoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ0123456789";//!@#$%^&*()-_=+{}[]'\\\"|,./<>?`";
static char const separator = '-';
#define ngroups 6
static int const groups[ngroups] = {4, 3, 4, 4, 3, 4};

static inline void shuffle(char *str) {
	static const char marker = '\0';
	int length = strlen(str);

	srand(time(NULL) * length + 119810710134500);

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

	str[length] = '\0';
}

static inline int passgen(char *out, const int outlen, const char *in, passgen_mode_t mode) {

	int length = strlen(in);	
	char shuffled[length + 1];
	strcpy(shuffled, in);
	for (int i = 0; i < length; ++i) {		
		shuffle(shuffled);
	}
	printf("%s\n", shuffled);

	// memcpy(out, shuffled, outlen - 1);

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

	out[outlen - 1] = '\0';

	return strlen(out);
}

int main(int argc, char const *argv[])
{

	passgen_mode_t mode = default_passgen_mode;

	char in[strlen(pool) + 1];
	in[strlen(in)] = 0;
	strcpy(in, pool);

	printf("%s\n", in);

	int outlen = ngroups;	
	for (int i = 0; i < ngroups; i++) {
		outlen += groups[i];
	}	
	
	char out[outlen];	
	passgen(out, outlen, in, mode);

	printf("%s\n", out);
	return 0;
}
