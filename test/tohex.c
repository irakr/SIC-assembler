#include <stdio.h>

#define TO_HEX(a) 0x##a

int main(){
	int x;
	scanf("%d",&x);
	printf("%d",TO_HEX(10));
	return 0;
}
