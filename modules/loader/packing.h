/*
 * This is free and unencumbered software released into the public domain.

 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.

 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.

 * For more information, please refer to <http://unlicense.org>
 */
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


