#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dhat.h"

int main(void)
{
	dhat ht;
	char *strs[] = {"str0", "str1", "str2", "str3", "str4", "str5", "str6"};
	char *str;

	ht = dhat_new(2, 2, 2);

	ht = dhat_put(ht, "0", strs[0]);
	ht = dhat_put(ht, "1", strs[1]);
	ht = dhat_put(ht, "2", strs[2]);
	ht = dhat_put(ht, "3", strs[3]);
	ht = dhat_put(ht, "4", strs[4]);
	ht = dhat_put(ht, "5", strs[5]);
	ht = dhat_put(ht, "6", strs[6]);

	if (dhat_get(ht, "6", (const void **) &str))
		printf("%s\n", str);
}
