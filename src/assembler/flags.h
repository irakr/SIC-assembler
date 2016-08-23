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
 *	flags.h
 *
 *	This file contains all the flag variables required for the execution of the assembler.
 */

#ifndef FLAGS_H
#define FLAGS_H

//Error flags
typedef enum{OK=0x00, EMPTY_OPCODE, INVALID_OPCODE, INVALID_LABEL, DUPLICATE_LABEL, INVALID_OPERAND} SIC_AsmError;
SIC_AsmError asm_error;

//Flags that indicates what type of instruction has been encountered.
typedef enum{INVALID=0xff, VALID=0x00, FORM1, FORM2, FORM3, FORM4, FORM0, COMMENT, EMPTY, MAYBE_DIRECTIVE} SIC_InstnType;
SIC_InstnType instn_type;

#endif

