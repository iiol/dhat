#ifndef _DHAT_H
#define _DHAT_H

struct entry {
	char *key;
	const void *value;
	struct entry *next;
};

typedef struct {
	unsigned int size;
	int maxdepth;
	int factor;
	struct entry **entry;
} *dhat;

dhat dhat_new(unsigned int size, int depth, int factor);
dhat dhat_put(dhat ht, char *key, const void *value);
int dhat_get(dhat ht, char *key, const void **value);
void dhat_remove(dhat ht, char *key);
void dhat_clear(dhat ht);

#endif //_DHAT_N
