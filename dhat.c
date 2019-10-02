#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include "dhat.h"


static inline void*
xmalloc(size_t size)
{
	void *ret;

	errno = 0;
	ret = malloc(size);

	if (size != 0 && ret == NULL) {
		perror("malloc()");
		exit(EXIT_FAILURE);
	}

	return ret;
}

static inline void*
xrealloc(void *ptr, size_t size)
{
	void *ret;

	ret = realloc(ptr, size);

	if (size != 0 && ret == NULL) {
		perror("realloc()");
		exit(EXIT_FAILURE);
	}

	return ret;
}

static struct entry*
ealloc(void)
{
	struct entry *tmp;

	tmp = xmalloc(sizeof (struct entry));
	memset(tmp, 0, sizeof (struct entry));

	return tmp;
}

//djb2 hash
static unsigned int
gethash_djb2(const void *s, size_t size, size_t mod)
{
	size_t i;
	size_t hash = 5381;

	for (i = 0; i < size; ++i)
		hash += (hash << 5) + ((int8_t*)s)[i];

	return hash % mod;
}

static size_t
gethash(dhat *ht, const wchar_t *s, size_t size, size_t mod)
{
	if (ht->gethash_external != NULL)
		return ht->gethash_external(s, size, mod);
	else
		return gethash_djb2(s, size, mod);

	return 0;
}

static dhat*
dhat_resize(dhat *ht)
{
	int i;
	struct entry *head;
	dhat *newht;

	newht = dhat_new(ht->factor * ht->size, ht->maxdepth, ht->factor);

	for (i = 0; i < ht->size; ++i) {
		head = ht->entry[i];

		for (; head != NULL; head = head->next)
			newht = dhat_nput(newht, head->key, head->keysize, head->value);
	}

	dhat_destroy(ht);

	return newht;
}

dhat*
dhat_new(unsigned int size, int depth, int factor)
{
	dhat *ht;

	ht = xmalloc(sizeof(dhat));
	memset(ht, 0, sizeof(dhat));
	ht->size = size;
	ht->maxdepth = depth;
	ht->factor = factor;

	size *= sizeof (struct entry*);
	ht->entry = xmalloc(size);
	memset(ht->entry, 0, size);

	return ht;
}

void
dhat_hashfunc(dhat *ht, size_t (*gethash)(const void *s, size_t size, size_t mod))
{
	ht->gethash_external = gethash;
}

dhat*
dhat_nput(dhat *ht, const void *key, size_t size, const void *value)
{
	int i;
	struct entry *head;
	unsigned int hash;

	hash = gethash(ht, key, size, ht->size);
	head = ht->entry[hash];

	for (i = 0;; head = head->next, ++i) {
		if (head == NULL) {
			head = ht->entry[hash] = ealloc();
			++i;
			break;
		}
		else if (size == head->keysize && !memcmp(head->key, key, size))
			break;
		else if (head->next == NULL) {
			head->next = ealloc();
			head = head->next;
			++i;
			break;
		}
	}

	head->key = xrealloc(head->key, size);
	head->keysize = size;
	head->keytype = VOID;
	memcpy(head->key, key, size);
	head->value = value;

	if (ht->maxdepth < i)
		ht = dhat_resize(ht);

	return ht;
}

int
dhat_nget(dhat *ht, const void *key, size_t size, void **value)
{
	struct entry *head;

	head = *(ht->entry + gethash(ht, key, size, ht->size));

	for (; head != NULL; head = head->next) {
		if (head->keysize == size && memcmp(head->key, key, size) == 0) {
			*value = (void*) head->value;

			return 1;
		}
	}

	return 0;
}

void
dhat_nremove(dhat *ht, const void *key, size_t size)
{
	size_t hash;
	struct entry *head, **phead;

	hash = gethash(ht, key, size, ht->size);
	head = ht->entry[hash];
	phead = ht->entry + hash;

	if (head == NULL)
		return;

	while (1) {
		if (!memcmp(key, head->key, size)) {
			*phead = head->next;
			free(head->key);
			free(head);
			break;
		}

		if (head->next == NULL)
			break;

		phead = &head->next;
		head = head->next;
	}
}

void
dhat_destroy(dhat *ht)
{
	int i;
	struct entry *head, *next;

	for (i = 0; i < ht->size; ++i) {
		head = ht->entry[i];

		for (; head != NULL; head = next) {
			next = head->next;
			dhat_nremove(ht, head->key, head->keysize);
		}
	}

	free(ht->entry);
	free(ht);
}
