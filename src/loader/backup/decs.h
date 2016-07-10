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
#define OBJCODESIZE 	3
#define MAX_PROG_LEN	32768	//32K = 2^15 bytes memory(Program + data). For SIC

#else
#error "Please include the file sic.h header"
#endif

//Constants common to both SIC and SIC/XE
#define	WORDSIZE 	3	//1 word = 3 bytes
#define OBJCODE_PER_TR	10	//Maximum objectcodes that can fit in a text record

//Error management
#include "errors.h"

//Collection of codes in character format
struct object_code_ch{
	char o_code[OBJCODE_PER_TR][OBJCODESIZE*2+1];	//Array of character format object codes
};

//Collection of codes in character format
struct object_code_bin{
	char o_code[OBJCODE_PER_TR][OBJCODESIZE+1];	//Array of character format object codes
};

//Program memory
typedef struct {
	struct object_code_bin content;	//Contents in the program memory are in binary form
} Program_memory;


/*	Program information encapsulated	*/
//Info extracted from header record
struct program_info{
	char p_name[7];	//Program name
	int p_start_addr;	//Starting address of the program
	int p_length;	//Program length
};

//Info extracted from text records
struct text_rec_info{
	int t_start_addr;	//Address of the first object code in a text record
	int t_nbytes;	//No of bytes occupied by all the object codes in a text record
	int t_ncount;	//No of object codes in a text record
	struct object_code_ch obj_codes;	//Collection of object codes in char format
};


#endif
