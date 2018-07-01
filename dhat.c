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

dhat dhat_new(unsigned int size)
{
	dhat ht;

	ht = xmalloc(sizeof(dhat));
	ht->size = size;

	size *= sizeof (struct entry *);

	ht->entry = xmalloc(size);
	memset(ht->entry, 0, size);

	return ht;
}

int dhat_add(dhat dhat, char *key, const void *value)
{
	struct entry *head;
	unsigned int hash;

	hash = gethash(key, dhat->size);

	head = dhat->entry[hash];

	for (;; head = head->next) {
		if (head == NULL) {
			head = dhat->entry[hash] = ealloc();

			break;
		}
		else if (head->next == NULL) {
			head->next = ealloc();
			head = head->next;

			break;
		}
	}

	head->key = xmalloc(strlen(key) + 1);
	strcpy(head->key, key);
	head->value = value;

	return 0;
}

int dhat_getdata(dhat dhat, char *key, const void **data)
{
	struct entry *head;

	head = *(dhat->entry + gethash(key, dhat->size));

	for (; head != NULL; head = head->next) {
		if (strcmp(head->key, key) == 0) {
			*data = head->value;

			return 1;
		}
	}

	return 0;
}
