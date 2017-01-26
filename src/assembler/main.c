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
 *	main.c
 *
 *	Written by:	Irak Rigia
 *	Date:	1 july 2016
 *	This is the main program for the SIC assembler module.
 *	This is a 2-pass assembler.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembly.h"
#include "optab.h"

#ifdef DEBUG
#include <unistd.h>
#endif

// This macro is not fully implemented yet. Currently it only builds optab. Lot more TODO
#define INIT_ASM()	build_optab();

/*	All global data		*/
SIC_Prog_info program_info;	//Program info as global variable

FILE *src_file;
FILE *inter_file;
FILE *list_file, *obj_prog_file;

int assemble();

int main(int argc, char *argv[]){
	
	/*	DEBUG	*/
	#ifdef DEBUG
	printf("Current working directory: %s\n", (char*)get_current_dir_name());	//print working directory
	#endif
	
	INIT_ASM();
	
	if(argc <= 1){
		//fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		fprintf(stderr,"\n[Error: No source filename provided]\n");
		exit(EXIT_FAILURE);
	}
	else if(argc == 2){
		//Check file extension and open input file
		if(!FILE_EXTENSION_SIC(argv[1])) {
			//fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
			fprintf(stderr,"\n[Error: Invalid source filename]\n");
			exit(EXIT_FAILURE);
		}
		if((src_file = fopen(argv[1],"r")) == NULL){
			//fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
			fprintf(stderr,"\n[Error: No file exists with the name '%s']\n",argv[1]);
			exit(EXIT_FAILURE);
		}
	}
	else {
		fprintf(stderr, "[Error: Please provide only one source file]\n");
		exit(EXIT_FAILURE);
	}
	
	//Program source filename
	strcpy(program_info.file_name, argv[1]);
	
	//Call assemble()
	assemble();
	
	return 0;
}

//This function invokes the actual algorithms.
int assemble(){
	
	//Call pass1()
	pass1();
	
	//Call pass2()
	//No need for arguments. Because required files are already made.
	pass2();
}
