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
int dhat_add(dhat dhat, char *key, const void *value);
int dhat_getdata(dhat dhat, char *key, const void **data);

#endif //_DHAT_N
