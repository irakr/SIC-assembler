#include <stdio.h>
#include "sic.h"

int main(){
	SIC_addr address;	//15 bits
	SIC_word word;		//24 bits(3 bytes)
	int a,b;
	while(1){
		printf("address = ");
		scanf("%d",&a);
		address.val = a;
		printf("word = ");
		scanf("%d",&b);
		word.val = b;
		printf("address = %d, word = %d\n", address.val, word.val);
	}
	return 0;
}
