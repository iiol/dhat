#include <stdio.h>

#include "dhat.h"

int main(void)
{
	dhat ht;
	char *str, *str2;
	int i = 500, *i2;

	str = malloc(100);
	strcpy(str, "Hello World");

	ht = dhat_new(10);
	dhat_put(ht, "100", &i);
	dhat_put(ht, "1000", str);

	if (dhat_get(ht, "100", (const void **) &i2))
		printf("int: %d\n", *i2);
	else
		printf("Данные не найдены.\n");

	if (dhat_get(ht, "1000", (const void **) &str2))
		printf("str: %s\n", str);
	else
		printf("Данные не найдены.\n");
}
