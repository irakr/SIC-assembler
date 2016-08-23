#include <stdio.h>

unsigned long
hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

int main(){
	char *str="ADD";
	printf("Hash(%s) = %ul\n", str, hash(str)/100000l);
	return 0;
}
