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

//Generates the 'SIC_Optab optab' table from the file 'optab.tab'
void build_optab() {
	FILE *optab_f = fopen("/etc/sasm/optab.tab", "r");
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
			optab[i].hexcode = (char*)malloc(sizeof(char)*strlen(temp));
			strcpy(optab[i].hexcode, temp);
		}
		
		++i;
	}//while end
	
	fclose(optab_f);
}


//Validates and returns the instruction format using optab.
SIC_InstnType validate(SIC_Source_line *srcline) {
	int i;
	
	char *optr = srcline->opcode;	// Maybe a '+' symbol prefix in opcode
	if(*optr == '+')
		optr++;
	
	//Find the entry for the current instruction in the optab
	for(i=0; i<tab_size; i++) {
		if(strstr(optab[i].mnemonic, optr))
			break;
	}
	if(i >= tab_size) {	//Not found
		
		#if DEBUG
		printf("[ Maybe directive ]");
		#endif
		
		return MAYBE_DIRECTIVE;
	}
	
	//Now validate...TODO!
	SIC_InstnType format;
	if(strchr(optab[i].format, '/')) {	//Format : 3/4
		format = (srcline->opcode[0] == '+') ? FORM4 : FORM3 ;
	}
	else					//Format : 1 or 2
		format = (optab[i].format[0] == '1') ? FORM1 : FORM2;
	#if DEBUG
	printf("[ Format %d ]", format);
	#endif
	
	return format;
}


/*	Get hexcode for 'opcode' from OPTAB	*/
char *get_hexcode(char *opcode){
	char *code=(char*)malloc(sizeof(char)*10);
	int i;
	
	if(opcode == (char*)0){	//If empty symbol
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return (char*)0;
	}
	
	//Linear search the opcode in optab
	for(i=0; i<tab_size; i++) {
		if(strstr(optab[i].mnemonic, opcode))	//found
			return optab[i].hexcode;
	}
	if(i >= tab_size)	//not found
		return NULL;
}

/*	Testing
int main(){
	build_optab();
	FILE *fp = fopen("test.sic", "r");
	SIC_Source_line srcline;
	while(!feof(fp)) {
		if(!read_line_src(&srcline)) {
			if(instn_type == COMMENT)
				#if DEBUG
				printf("[ Comment line ]");
				#endif
			else if(instn_type == EMPTY)
				#if DEBUG
				printf("[ Empty line ]");
				#endif
			else
				validate(&srcline);
		}
		#if DEBUG
		printf("\n");
		#endif
	}
	return 0;
}
*/






