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
 *	decs.h
 *
 *	This file contains the declarations of some user-defined data types for the purpose of 
 *	loading a program and with some macros.
 */

#ifndef DECS_H_
#define DECS_H_

/*	Maximum program size in bytes	*/
//SIC_XE version
#ifdef SIC_XE_STD_
#define MAX_PROG_LEN	1048576	//1024K = 2^20 bytes memroy(Program + data). For SIC/XE
//SIC version
#elif defined(SIC_STD_)
#define OBJCODE_SIZE 	3
#define MAX_PROG_LEN	32768	//32K = 2^15 bytes memory(Program + data). For SIC

#else
#error "Please include the file sic.h header"
#endif

//Constants common to both SIC and SIC/XE
#define	WORDSIZE 	3	//1 word = 3 bytes
#define OBJCODE_PER_TR	10	//Maximum objectcodes that can fit in a text record

//Error management
#include "errors.h"

/*
//Collection of codes in character format
struct object_code_ch{
	char o_code[OBJCODE_PER_TR+1][OBJCODE_SIZE*2+1];	//Array of character format object codes
};

//Collection of codes in character format
struct object_code_bin{
	char o_code[OBJCODE_PER_TR][OBJCODE_SIZE+1];	//Array of internal binary format object codes
};
*/

//Object codes in extracted from each text record and their associated addresses embedded
struct object_codes{
	int *o_addr;	//Array of first addresses of each word(instructions or data or reserved memory space)
	char **o_code;	//Array of character format object codes
};


//Program memory
typedef struct {
	char **content;	//Contents in the program memory are in binary form
} Program_memory;


/*	Program information encapsulated	*/
//Info extracted from header record
struct program_info{
	char p_name[7];	//Program name
	int p_start_addr;	//Starting address of the program
	int p_nbytes;	//Program length in bytes
	int p_ncount;	//No. of words in the program
};

//Info extracted from text records
struct text_rec_info{
	int t_start_addr;	//Address of the first object code in a text record
	int t_nbytes;	//No of bytes occupied by all the object codes in a text record
	int t_ncount;	//No of object codes in a text record
};


#endif
