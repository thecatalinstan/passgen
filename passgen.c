#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int main(int argc, char const *argv[])
{

	

	char in[strlen(pool) + 1];
	strcpy(in, pool);
	in[strlen(in)] = 0;
	printf("%s\n", in);

	int length = strlen(in);	
	char *shuffled = malloc(length + 1);
	strcpy(shuffled, in);
	for (int i = 0; i < length; ++i) {		
		shuffle(shuffled);
	}

	printf("%s\n", shuffled);

	int outlength = ngroups;
	for (int i = 0; i < ngroups; i++) {
		outlength += groups[i];
	}

	char out[outlength];	

	// memcpy(out, shuffled, outlength - 1);

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

	out[outlength - 1] = '\0';

	printf("%s\n", out);

	free(shuffled);
	return 0;
}
