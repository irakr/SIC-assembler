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
 * optab.h
 *
 * Contains runtime optab data structured. The optab defined in this file is a hash table.
 */

#include <sys/types.h>
#include "assembly.h"
#include "flags.h"

#ifndef OPTAB_H
#define OPTAB_H


//Table of opcodes
typedef struct {
	unsigned char *mnemonic, *format, *hexcode;
} SIC_Optab;

//Runtime optab table
SIC_Optab *optab;

int16_t tab_size;	//Default = 59

//Generates the 'SIC_Optab optab' table from the file 'optab.tab'
void build_optab();

//Validates and returns the instruction format using optab.
SIC_InstnType validate(SIC_Source_line*);

//Get hexcode for 'opcode' from OPTAB
char *get_hexcode(char *opcode);

#endif
