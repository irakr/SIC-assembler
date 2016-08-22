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
 *	errors.h
 *
 *	Error checking utility header file.
 */

#ifndef ERRORS_H_
#define ERRORS_H_

//Macro for setting an error flag
#define SETERROR(ERRNO)	_errno=ERRNO
#define RESETERROR()	_errno=NO_ERROR

//Error flag. Maybe raised by any of the modules in the software.
enum{NO_ERROR=0,ERR_CORRUPT_FILE,ERR_MEMORY_LEAK,ERR_PROGSIZE} _errno;

//Error flags for assembly time checkings.............. Eg: Invalid opcode,duplicate symbol,etc
//.........To be implemented

//Check for error and generate report
void check_error(const char *module_name);


#endif
