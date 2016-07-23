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
