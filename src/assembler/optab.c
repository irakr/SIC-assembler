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
 * optab.c
 *
 * Definitions of functions that process the optab defined in optab.h.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "optab.h"

#define ASCII_3	51
#define ASCII_4 52

//Generates the 'SIC_Optab optab' table from the file 'optab.tab'
void build_optab() {
	FILE *optab_f = fopen("optab.tab", "r");
	char line[256];
	int valid_lines = 0, i=0;
	
	if(optab_f == NULL) {
		perror("fopen");
		fprintf(stderr, "[ ERROR: Unable to open optab file ]\n");
		exit(1);
	}
	
	//Analyze the actual optab
	while(!feof(optab_f)) {
		fgets(line, 256, optab_f);
		line[strlen(line)-2] = '\0';
		if((*line == '\0') || (*line == '#'))	//Ignore comment and blank lines
			continue;
		valid_lines++;
	}
	//printf("%d lines of instruction entries found.\n",valid_lines);
	
	optab = (SIC_Optab*)calloc(valid_lines, sizeof(SIC_Optab));
	if(optab == NULL) {
		perror("calloc");
		fprintf(stderr,"[ ERROR: Unable to allocate memory for runtime optab ]\n");
		exit(1);
	}
	tab_size = valid_lines;
	
	rewind(optab_f);
	
	//Generate the runtime optab now
	while(!feof(optab_f) && (i<valid_lines)) {
		char *temp;
		fgets(line, 256, optab_f);
		line[strlen(line)-2] = '\0';
		if((*line == '\0') || (*line == '#'))	//Ignore comment and blank lines
			continue;
		
		//Mnemonic part
		temp = strtok(line, "\t");
		if(temp) {
			optab[i].mnemonic = (char*)malloc(sizeof(char)*strlen(temp));
			strcpy(optab[i].mnemonic, temp);
		}
		//Format part
		temp = strtok(NULL,"\t");
		if(temp) {
			optab[i].format = (char*)malloc(sizeof(char)*strlen(temp));
			strcpy(optab[i].format, temp);
		}
		//Opcode part
		temp = strtok(NULL, "\t");
		if(temp) {
			optab[i].opcode = (char*)malloc(sizeof(char)*strlen(temp));
			strcpy(optab[i].opcode, temp);
		}
		
		++i;
	}//while end
	
	fclose(optab_f);
}


//Validates and returns the instruction format using optab.
SIC_InstnType validate(SIC_Source_line *srcline) {
	int i;
	
	//Find the entry for the current instruction in the optab
	for(i=0; i<tab_size; i++) {
		if(strstr(optab[i].mnemonic, srcline->opcode))
			break;
	}
	if(i >= tab_size) {	//Not found
		printf("[ Maybe directive ]");
		return MAYBE_DIRECTIVE;
	}
	
	//Now validate...TODO!
	SIC_InstnType format;
	if(strchr(optab[i].format, '/')) {	//Format : 3/4
		format = (srcline->opcode[0] == '+') ? FORM4 : FORM3 ;
	}
	else					//Format : 1 or 2
		format = (optab[i].format[0] == '1') ? FORM1 : FORM2;
	printf("[ Format %d ]", format);
	
}

//Testing
int main(){
	build_optab();
	FILE *fp = fopen("test.sic", "r");
	SIC_Source_line srcline;
	while(!feof(fp)) {
		if(!read_line_src(fp, &srcline)) {
			if(instn_type == COMMENT)
				printf("[ Comment line ]");
			else if(instn_type == EMPTY)
				printf("[ Empty line ]");
			else
				validate(&srcline);
		}
		printf("\n");
	}
	return 0;
}







