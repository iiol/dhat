#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dhat.h"

static void *xmalloc(size_t size)
{
	void *vp;

	errno = 0;
	vp = malloc(size);

	if (size != 0 && vp == NULL) {
		perror("markov");

		exit(EXIT_FAILURE);
	}

	return vp;
}

static void *xrealloc(void *p, size_t size)
{
	void *vp;

	errno = 0;
	vp = realloc(p, size);

	if (size != 0 && vp == NULL) {
		perror("markov");

		exit(EXIT_FAILURE);
	}

	return vp;
}

static struct entry *ealloc(void)
{
	struct entry *tmp;

	tmp = xmalloc(sizeof (struct entry));
	memset(tmp, 0, sizeof (struct entry));

	return tmp;
}

//djb2 hash
static unsigned int gethash(const char *s, unsigned int size)
{
	unsigned int i;
	unsigned int hash = 5381;

	for (i = 0; s[i] != '\0'; ++i)
		hash += (hash << 5) + s[i];

	return hash % size;
}

static dhat dhat_resize(dhat ht) {
	int i;
	struct entry *head;
	dhat newht;

	newht = dhat_new(ht->factor * ht->size, ht->maxdepth, ht->factor);

	for (i = 0; i < ht->size; ++i) {
		head = ht->entry[i];

		for (; head != NULL; head = head->next)
			newht = dhat_put(newht, head->key, head->value);
	}

	dhat_clear(ht);

	return newht;
}

dhat dhat_new(unsigned int size, int depth, int factor)
{
	dhat ht;

	ht = xmalloc(sizeof(dhat));
	ht->size = size;
	ht->maxdepth = depth;
	ht->factor = factor;

	size *= sizeof (struct entry *);

	ht->entry = xmalloc(size);
	memset(ht->entry, 0, size);

	return ht;
}

dhat dhat_put(dhat ht, char *key, const void *value)
{
	int i;
	struct entry *head;
	unsigned int hash;

	hash = gethash(key, ht->size);

	head = ht->entry[hash];

	for (i = 1;; head = head->next, ++i) {
		if (head == NULL) {
			head = ht->entry[hash] = ealloc();

			break;
		}
		else if (strcmp(head->key, key) == 0)
			break;
		else if (head->next == NULL) {
			head = head->next = ealloc();
			++i;

			break;
		}
	}

	head->key = xrealloc(head->key, strlen(key) + 1);
	strcpy(head->key, key);
	head->value = value;

	if (ht->maxdepth < i)
		ht = dhat_resize(ht);

	return ht;
}

int dhat_get(dhat ht, char *key, const void **value)
{
	struct entry *head;

	head = *(ht->entry + gethash(key, ht->size));

	for (; head != NULL; head = head->next) {
		if (strcmp(head->key, key) == 0) {
			*value = head->value;

			return 1;
		}
	}

	return 0;
}

void dhat_remove(dhat ht, char *key)
{
	unsigned int hash;
	struct entry *head, **phead;

	hash = gethash(key, ht->size);
	head = ht->entry[hash];
	phead = ht->entry + hash;

	for (; head != NULL; phead = &head->next, head = head->next) {
		if (strcmp(key, head->key) == 0) {
			*phead = head->next;
			free(head->key);
			free(head);
		}
	}
}

void dhat_clear(dhat ht)
{
	int i;
	struct entry *head, *next;

	for (i = 0; i < ht->size; ++i) {
		head = ht->entry[i];

		for (; head != NULL; head = next) {
			next = head->next;

			dhat_remove(ht, head->key);
		}
	}

	free(ht);
}
