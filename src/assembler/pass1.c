/*
 *	This is the full implementation of pass-1 of the SIC assembler
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"

#define SINGLEQUOTE 0x27	//The symbol '

extern SIC_Prog_info program_info;


/*	Pass-1 assembly algorithm	*/
void pass1(FILE *src_file){
	FILE *intr_file, *symtab;
	SIC_Source_line src_line;		//Lines read from source file
	SIC_Interm_line intr_line;	//Lines written to intermediate file
	
	SIC_Integer bytes,inc,const_len;
	SIC_Addr locctr;

//Macros for writting structs in shortcut
#define bytes 		(bytes.val)
#define locctr 		(locctr.val)
#define inc 		(inc.val)
#define const_len 	(const_len.val)

	//Symbol table for the current source file.
	if((symtab = fopen(".symtab","w")) == NULL){
		fprintf(stderr,"\nError: Could not open SYMTAB.\n");
		exit(EXIT_FAILURE);
	}
	
	//Generating output(intermediate) filename
	if((intr_file=fopen(".intermediate","w")) == NULL){
		fprintf(stderr,"Error: Could not open intermediate file\n");
		exit(1);
	}
	
	read_line_src(src_file, &src_line);
	strcpy(program_info.prog_name, src_line.label);	//Get program name(or procedure name)

	intr_line.instr = &src_line;	//Partial intermediate line constructed
	
#define start_addr	start_addr.val

	/* opcode=="START"	*/
	if(strcmp(src_line.opcode,"START")==0){
		uint16_t temp;
		sscanf(src_line.operand, "%x", &temp);
		program_info.start_addr = temp;
		locctr = program_info.start_addr;
		intr_line.addr.val = temp;
		write_line_intr(intr_file, &intr_line);	//Define these two functions
		read_line_src(src_file, &src_line);	//Second line onwards
	}
	else	locctr = 0;

	/*	Read rest of the lines	*/	
	while(strcmp(src_line.opcode,"END")!=0){
		if(strcmp(src_line.label,".") == 0){	//If comment line
			read_line_src(src_file, &src_line);
			continue;
		}

		inc = 0;	//increment value for locctr
		if(strcmp(src_line.label,"-")!=0){//If there is something in the label field
			if(search_symtab(src_line.label) == FOUND){	//Symbol already defined
				fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
				fprintf(stderr,"\nError: Symbol '%s' is already defined.\n",src_line.label);
			}
			else{	//Insert the (symbol,locctr) pair into the SYMTAB
				fprintf(symtab, "%s\t%X\n", src_line.label, locctr);
			}//if search_symtab()
			fflush(symtab);
		}//if label is not empty(Not an error)
		//else	fprintf(stderr,"\nEmpty label.\n");
		
		/*	Search OPTAB for opcode	*/
		if(search_optab(src_line.opcode) == FOUND){//Search OPTAB for opcode
			inc += WORDSIZE;
			//printf("\nOpcode is valid.\n");
		}
		/* Check the opcode field with various assembler directives	*/
		else if(strcmp(src_line.opcode,"WORD")==0)
			inc = WORDSIZE;
		else if(strcmp(src_line.opcode,"RESW")==0)
			inc = (WORDSIZE * atoi(src_line.operand));
		else if(strcmp(src_line.opcode,"RESB")==0)
			inc = atoi(src_line.operand);
		else if(strcmp(src_line.opcode,"BYTE")==0){
			if(src_line.operand[0] == 'C')//For character constants
				const_len = strlen(index(src_line.operand,SINGLEQUOTE)) - 2;		//Excluding ' from the string 'EOF'
			else if(src_line.operand[0] == 'X')//For hexadecimal constants
				const_len = 1;
			else{
				//@ERR: invalid_const_f = 1;	//Invalid constant type flag
				fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
				fprintf(stderr,"\nError: Invalid constant type '%s'\n", src_line.operand);
			}
			inc = const_len;
		}
		else{
			//@ERR: invalid_op_f = 1;	//Invalid opcode flag
			fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
			fprintf(stderr,"\nError: Invalid opcode.\tAt label: %s\n",src_line.label);
			exit(1);
		}//if(search OPTAB)
		//printf("-----------------------------------------------\n");
		
		//Only change address member of intr_line. That's why I used the 2nd member as a pointer to src_line.
		intr_line.addr.val = locctr;
		
		write_line_intr(intr_file, &intr_line);//Write line to intermediate file
		locctr += inc;	//Update locctr now
		read_line_src(src_file, &src_line);	//Read next input line
		//reset_flags();	//Reset all flags
	}//while()
	intr_line.addr.val = locctr;
	write_line_intr(intr_file, &intr_line);	//Write last line
	program_info.prog_len = (locctr - program_info.start_addr) + 1;
	//printf("\nLength of program = %d(DEC) or %X(HEX)\n",prog_len,prog_len);
	fclose(src_file);
	fclose(symtab);
	fclose(intr_file);
}
