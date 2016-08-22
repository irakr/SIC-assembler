/*
 *	loader.c
 *
 *	Description: Source code of the loader program for the SIC assembler.
 *
 *	Written by:	Irak Rigia
 *	Date:	22nd May 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include "/home/irakr/Programs/SystemProgramming/SIC_assembler/src/includes/sic.h"
#include "decs.h"
#include "packing.h"

//View the information in the program_info structure
void display_prog_info(struct program_info);

//Set program info to struct prog_info p from header record line just read
int set_prog_info(char*, struct program_info*);

//Set text record info to struct text_rec_info from text record line just read
int set_text_rec_info(char*, struct text_rec_info*);

//Load all the program instructions(object codes) to memory
void load_program();

//Main
int main(int argc,char *argv[]){
	FILE *object_prog_char,*object_prog_bin;
	char prog_name[7]="";
	char line[64]="";
	u_char obj_code[3]="";	//1 word object code = 24-bits or 3-bytes
	char *program_mem;	//Memory block where the program will be loaded
	struct program_info prog_info;
	struct text_rec_info text_rec;

	//Reset error flags
	RESETERROR();

	//No argv's
	if(argc <= 1){
		fprintf(stderr,"Error: No file provided\n");
		exit(1);
	}
	
	//open file. Input object program must be in character form, i.e., each charater takes 1 byte.
	if((object_prog_char = fopen(argv[1],"r")) == NULL){
		perror("fopen");
		exit(1);
	}
	//The output object program will contain the compressed version of input object program
	if((object_prog_bin = fopen("test.obj","w")) == NULL){
		perror("fopen");
		fprintf(stderr,"'test.obj'");
		exit(1);
	}
	
	//Extract program info from header record
	if(fgets(line,sizeof(line),object_prog_char) <= 0){
		perror("fgets");
		exit(1);
	}
	set_prog_info(line,&prog_info);
	//display_prog_info(prog_info);
	
	//Allocate memory of size p_length for the program to be loaded and check for size limit(max = 32K)
	if(prog_info.p_length >= MAX_PROG_LEN){
		SETERROR(ERR_PROGSIZE);
		goto error;
		//fprintf(stderr,"Error: Memory for current program is not available.\n	\
		Maximum memory available = %d\tCurrent program size = %d\n",MAX_PROG_LEN,prog_info.p_length);
		//exit(1);
	}
	else{	//Valid program length
		if((program_mem = (char*)calloc(prog_info.p_length+1,1)) == NULL){
			SETERROR(ERR_MEMORY_LEAK);
			goto error;
		}
	}
	
	if(fgets(line,sizeof(line),object_prog_char) <= 0){	//Probably first text record
		SETERROR(ERR_CORRUPT_FILE);
		goto error;
		//break;
	}
	//Examine text records
	while(line[0]!='E') {

		//Extract text record infos and object codes
		if(set_text_rec_info(line,&text_rec) == -1){
			goto error;
			//break;
		}

		//Read next line
		if(fgets(line,sizeof(line),object_prog_char) <= 0){	//No more data readable or EOF
			SETERROR(ERR_CORRUPT_FILE);
			goto error;
			//break;
		}
		
	}//endwhile
	
	//TODO:	Finally load the complete program in memory
	
	//TODO Error checking
error:	check_error(argv[0]);

	fclose(object_prog_char);
	fclose(object_prog_bin);
	
	return 0;
}

//Set program info to struct prog_info p from header record line just read
int set_prog_info(char *hdr_rec,struct program_info *p){
	if(hdr_rec[0] != 'H'){	//if object program is invalid. (No header record found).This error will probably never occur.
		fprintf(stderr,"Error: First argument must be a header record.\n");
		return -1;
	}
	
	char *token;
	//printf("%s\n",strtok(line,"^"));
	token = strtok(hdr_rec,"^");
	token = strtok(NULL,"^");
	strncpy(p->p_name,token,strcspn(token," "));	//Program name
	token = strtok(NULL,"^");
	sscanf(token,"%x",&p->p_start_addr);	//Program starting address
	token = strtok(NULL,"^");
	sscanf(token,"%x",&p->p_length);	//Program length(bytes)

	return 0;
}

//Set text record info to struct text_rec_info from text record line just read
int set_text_rec_info(char *text_rec, struct text_rec_info *t){

	if(text_rec[0] != 'T'){	//Verify correct text record
		fprintf(stderr,"Error: First argument must be a text record.\n");
		return -1;
	}
	
	char *token;
	token = strtok(text_rec,"^");	//T
	token = strtok(NULL,"^");	//first_addr
	sscanf(token,"%x",&t->t_start_addr);
	token = strtok(NULL,"^");	//no of bytes of object codes
	sscanf(token,"%x",&t->t_nbytes);
	t->t_ncount = t->t_nbytes / OBJCODESIZE;

	/* Allocate memory for struct object_code_ch
	t->obj_codes.o_code = (char**)malloc(t->t_ncount * sizeof(char*));
	if(t->obj_codes.o_code == NULL){
		SETERROR(ERR_MEMORY_LEAK);
		return -1;
	}
	for(int i=0; i<t->t_ncount; i++){
		t->obj_codes.o_code[i] = (char*)malloc(OBJCODESIZE);
		if(t->obj_codes.o_code == NULL){
			SETERROR(ERR_MEMORY_LEAK);
			return -1;
		}
	}	*/

	int i=0;
	//Extract object codes
	while(token != NULL){
		token = strtok(NULL,"^"); //On first iteration, the 1st object code will be extracted
		token[6] = '\0';	//Replace \n with \0
		strcpy(t->obj_codes.o_code[i],token);
		++i;
		
	}
	return 0;
}

//View the information in the program_info structure
void display_prog_info(struct program_info p){
	printf("p.p_name = %s\np.p_length = %X\np.p_start_addr = %X\n",p.p_name,p.p_length,p.p_start_addr);
}
