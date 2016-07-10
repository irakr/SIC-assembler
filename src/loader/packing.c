/******************************** DEFINITIONS *****************************************/
/*	Pack 'a' and 'b' considering a holding the higher order bits and b holding
 *	lower order bits of the resulting number
 */
#include "packing.h"

u_char pack(u_char a,u_char b){
	u_char result;
	//a and b should not be larger than 0x0f or 00001111b
	if((a > 0x0f) || (b > 0x0f))
		return (u_char)-1;
	result = a << 4;
	result += b;
	return result;
}

/* Unpack 'a' into two separate u_characters and return them as malloced array of size 2 */
u_char *unpack(u_char a){
	u_char *result = (u_char*)calloc(2,sizeof(u_char));
	result[1] = a & 0x0f;	//Lower byte
	result[0] = a & 0xf0;	//Higher byte
	result[0] >>= 4;
	return result;
}
