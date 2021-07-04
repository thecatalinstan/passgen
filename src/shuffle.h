//
//  shuffle.h
//  passgen (lib)
//
//  Created by Cătălin Stan on 04/07/2021.
//

#ifndef shuffle_h
#define shuffle_h

#include <stdlib.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void reshuffle_pool(char *pool, size_t poollen);

#endif /* shuffle_h */
