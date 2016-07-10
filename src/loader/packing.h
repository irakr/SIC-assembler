/*
 *	packing.c
 *	Packing two numbers.
 */
#include <stdio.h>
#include <stdlib.h>
//#include <strings.h>

#ifndef PACKING_H_
#define PACKING_H_

//Pack 'a' and 'b' considering a holding the higher order bits and b holding lower order bits
u_char pack(u_char a,u_char b);

//Unpack 'a' into two separate u_characters and return them as malloced array of size 2
u_char *unpack(u_char a);



#endif

/**************	Main --- TEST ************************
int main(){
	int num1,num2;
	u_char packed,*unpacked;
	printf("Sizeof(num1) = %lu\n",sizeof(num1));
	printf("Enter two numbers in HEX : ");
	scanf("%i %i",&num1,&num2);
	printf("num1 = %XH\tnum2 = %XH\n",num1,num2);
	packed = pack(num1,num2);
	if(packed == -1){
		printf("Error: %d\n",packed);
		exit(1);
	}
	printf("sizeof(packed) = %lu\n",sizeof(packed));
	printf("Packed number = %XH\n",packed);
	unpacked = unpack(packed);
	printf("Unpacked numbers = %XH and %XH\n",unpacked[0],unpacked[1]);
	return 0;
}
*/


