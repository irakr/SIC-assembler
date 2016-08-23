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

#include <stdio.h>
#include <stdlib.h>
#include "_string.h"

/* Replace all the characters defeined by 'on' with the character 'put' in the string 'str'.
 * Returns the no of characters replaced.
 * More than one consecutive white spaces will be replaced by one 'put' only.
 */
size_t strput(char *str, char on, char put){	//ALL OK!
	char *ptr;
	size_t count=0;
	
	for(ptr=str; *ptr; ptr++){
		if(*ptr == on){
			*ptr = put;
			count++;
			
			//For white spaces	Ex: "Hello-   World!"
			if(on == ' '){
				ptr++;
				if(*ptr == 0)	//Reached NULL
					break;
				char *temp_ptr = ptr;
				for(; *ptr==' '; ptr++); //Find the next non-space character.
				if(*ptr=='\0')
					*temp_ptr = '\0';
				else
					memmove((char*)temp_ptr, (char*)ptr, strlen(ptr)+1);
				ptr = temp_ptr;
			}

		}
	}
	
	return count;
}

/*
int main(){
	char str[]="Hello   World!   a   as   ";
	printf("Original str[] = %s\n", str);
	int c = strput(str, ' ', '\t');
	printf("%d bytes were replaced.\nNew str[] = '%s'\n", c, str);
	return 0;
}
*/
