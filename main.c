#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dhat.h"

int main(void)
{
	dhat *ht;
	wchar_t *strs[] = {L"str0", L"str1", L"str2", L"str3", L"str4", L"str5", L"str6"};
	wchar_t *str;

	ht = dhat_new(2, 2, 2);

	ht = dhat_wput(ht, L"0", strs[0]);
	ht = dhat_wput(ht, L"1", strs[1]);
	ht = dhat_wput(ht, L"2", strs[2]);
	ht = dhat_wput(ht, L"3", strs[3]);
	ht = dhat_wput(ht, L"4", strs[4]);
	ht = dhat_wput(ht, L"5", strs[5]);
	ht = dhat_wput(ht, L"6", strs[6]);

	if (dhat_wget(ht, L"6", (void**)&str))
		printf("%ls\n", str);

	dhat_destroy(ht);
}
