#ifndef _DHAT_H
#define _DHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct entry {
	char *key;
	const void *value;
	struct entry *next;
};

typedef struct {
	unsigned int size;
	struct entry **entry;
} *dhat;

dhat dhat_new(unsigned int size);
void dhat_put(dhat dhat, char *key, const void *value);
int dhat_get(dhat dhat, char *key, const void **data);
// dhat_remove
// dhat_clear


#endif //_DHAT_N
