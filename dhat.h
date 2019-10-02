#ifndef _DHAT_H
#define _DHAT_H

#include <wchar.h>

enum key_type {
	WCHAR,
	CHAR,
	VOID,
};

struct entry {
	enum key_type keytype;
	size_t keysize;
	wchar_t *key;
	const void *value;
	struct entry *next;
};

typedef struct dhat {
	unsigned int size;
	int maxdepth;
	int factor;
	struct entry **entry;

	size_t (*gethash_external)(const void *s, size_t size, size_t mod);
} dhat;

dhat *dhat_new(unsigned int size, int depth, int factor);
void dhat_hashfunc(dhat *ht, size_t (*gethash)(const void *s, size_t size, size_t mod));

#define dhat_put (ht, key, val) dhat_nput(ht, key, (strlen(key)+1) * sizeof (char),    val)
#define dhat_wput(ht, key, val) dhat_nput(ht, key, (wcslen(key)+1) * sizeof (wchar_t), val)
dhat *dhat_nput(dhat *ht, const void    *key, size_t size, const void *value);

#define dhat_get (ht, key, val) dhat_nget(ht, key, (strlen(key)+1) * sizeof (char),    val)
#define dhat_wget(ht, key, val) dhat_nget(ht, key, (wcslen(key)+1) * sizeof (wchar_t), val)
int dhat_nget(dhat *ht, const void    *key, size_t size, void **value);

#define dhat_remove (ht, key) dhat_nremove(ht, key, (strlen(key)+1) * sizeof (char))
#define dhat_wremove(ht, key) dhat_nremove(ht, key, (wcslen(key)+1) * sizeof (wchar_t))
void dhat_nremove(dhat *ht, const void *key, size_t size);

void dhat_destroy(dhat *ht);

#endif //_DHAT_N
