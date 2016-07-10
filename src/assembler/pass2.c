#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembly.h"

#define SINGLEQUOTE 0x27	//The symbol '
#define MAXTRLEN	0x1E	//Max bytes of object codes a text record can hold

extern SIC_prog_info program_info;

//Generate address portion of an object by finding operands address from symtab. Also check that address does not croos the limit MAX_ADDR.
#define generate_objcode_addr(symbol)		uint16_t temp;	\
					sscanf(get_symbol_addr(symbol),"%X",&temp);	\
					if(temp >= MAX_ADDR){	\
						fprintf(stderr,"Error: Program size too large\n");	\
							exit(1);	\
					}	\
					hex_addr = temp;

/*	Pass-2 assembly algorithm	*/
void pass2() {
	FILE *intr_file, *listing, *obj_prog, *src_file;
	char symval[10]="", obj_code[10]="";
	char *objprog_name, text_rec[81]="";	//81bytes is enough to hold a text record. 
	int txtrec_ctr; //Text record counter
	int reswb_f = 0;	//RESW or RESB found flag. A special flag used during object program production by pass2 only

	SIC_addr hex_addr;	//program length is just taken as fixed for now.
	SIC_reg index = 0x08;	//X = 0000 1000(Just taken)
	SIC_src_instr src_line;	//Part of intr_line
	SIC_int_instr intr_line;	//Lines read from intermediate file
	SIC_list_instr list_line;	//Lines written to listing file
	
	intr_line.instr = &src_line;	//Associate src_line to intr_line for use
	list_line.instr = &intr_line;	//Associate intr_line to list_line for use
	strcpy(list_line.obj_code,"");	//Clear the objcode[] member
	
#define hex_addr	(hex_addr.val)

	//Input source file
	if((src_file = fopen(program_info.file_name,"r")) == NULL){
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		fprintf(stderr,"\nError: Could not open source file\n");
		exit(1);
	}
	if((intr_file = fopen(".intermediate","r")) == NULL){
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		fprintf(stderr,"\nError: Could not load intermediate file\n");
		exit(1);
	}
	if((listing = fopen(".listing","w")) == NULL){	//Assembly listing file
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		fprintf(stderr,"Error: Cannot open listing file\n");
		exit(1);
	}
	
	objprog_name = (char*)calloc(strlen(program_info.file_name),sizeof(char));
	strncpy(objprog_name, program_info.file_name, strcspn(program_info.file_name, "."));
	strcat(objprog_name,".o");
	//Object program file
	if((obj_prog = fopen(objprog_name,"w")) == NULL){
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		fprintf(stderr,"\nError: Could not open object file\n");
		exit(1);
	}
	//reset_flags();	//Reset all flags
	
	//Read first line of intermediate file
	read_line_intr(intr_file, &intr_line);
	
	//First startement of the source program
	if(strcmp(intr_line.instr->opcode,"START")==0){
		write_line_list(listing, &list_line);

		//Write object program header record
		fprintf(obj_prog, "H^%-6s^%06X^%06X\n", program_info.prog_name, program_info.start_addr, program_info.prog_len);

		read_line_intr(intr_file, &intr_line);
	}
	
	//Initialize first text record
	fprintf(obj_prog,"T^%06X^",intr_line.addr.val);
	fflush(obj_prog);
	
	//Scan intermediate file till 'END' statement or End of file(EOF)
	while((strcmp(intr_line.instr->opcode,"END")!=0) && !feof(intr_file)){
		//If comment line
		if(strcmp(intr_line.instr->label,".") == 0){
			read_line_intr(intr_file, &intr_line);
			continue;
		}
		//Else parse line....
		if(search_optab(intr_line.instr->opcode) == FOUND){	//Opcode found
			if(strcmp(intr_line.instr->operand,"-") != 0){//Not an empty label field
				if(search_symtab(intr_line.instr->operand) == FOUND){//Symbol found
					generate_objcode_addr(intr_line.instr->operand);
				}
				else if(strstr(intr_line.instr->operand,",X") != (char*)0){	//For indexed addressing, add value of X to address
					char temp_sym[10]="";
					//uint16_t temp;
					SIC_addr temp_addr;
					//Extract the symbol(excluding ',X') from the operand field
					strncpy(temp_sym,intr_line.instr->operand,strcspn(intr_line.instr->operand,",X"));
					if(search_symtab(temp_sym) == NOTFOUND){
						fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
						fprintf(stderr,"Error: Invalid symbol '%s'\n", temp_sym);
						exit(1);
					}
					
					generate_objcode_addr(temp_sym);	//Address of label at temp_sym(not original operand)
					hex_addr += index;	//Add hex_addr+index_register. Computed as integers
				}
				else{	//Invalid symbol
					hex_addr = 0;
					//@ERR: sym_f = 1;	//Invalid symbol flag
				}
			}//operand "-"
			else
				hex_addr = 0;
				//@strcpy(symval,"0000");
				//sscanf("0","%X",&hex_addr);
			
			//Make the complete object code
			sprintf(symval,"%04X",hex_addr);
			//printf("symval: %s\thex_addr: %d\n",symval,hex_addr);
			strcpy(obj_code,get_hexcode(intr_line.instr->opcode));	// 04
			//@strcat(obj_code,symval);
			strcat(obj_code,symval);	//04400C
			strcpy(list_line.obj_code, obj_code);	//Copy it to list_line struct member for write_line_list() operation.
			
		}//end if opcode found
		else if(strcmp(intr_line.instr->opcode,"WORD")==0){
			//uint16_t temp;
			//sscanf(intr_line.instr->operand,"%X",&temp);
			hex_addr = intr_line.addr.val;
			sprintf(obj_code,"%06X",hex_addr);
			strcpy(list_line.obj_code, obj_code);	//Copy it to list_line struct member for write_line_list() operation.
		}
		else if(strcmp(intr_line.instr->opcode,"BYTE")==0){
			char temp[10]="";	//Temp constant value variable
			if(intr_line.instr->operand[0] == 'C'){	//Eg: C'EOF'
				char *ptr;
				ptr = strstr(intr_line.instr->operand,"'") + 1;
				if(strcmp(ptr,"EOF'") == 0)
					strcpy(temp, "454F46");
				//Any other character constants can be added here.....TODO
				else{
					fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
					fprintf(stderr,"Error: Invalid constand type\tAt label: %s\n",intr_line.instr->label);
					exit(1);
				}
			}
			else if(intr_line.instr->operand[0] == 'X'){	//Eg: X'F1'
				char *ptr;
				short int i=0;
				ptr = strstr(intr_line.instr->operand,"'") + 1;
				while(*ptr != SINGLEQUOTE){
					temp[i] = *ptr;
					ptr++; i++;
				}
				temp[i] = '\0';
			}
			strcpy(symval, temp);
			strcpy(obj_code, symval);
			strcpy(list_line.obj_code, obj_code);	//Copy it to list_line struct member for write_line_list() operation.
		}
		//For RESW and RESB
		else if((strcmp(intr_line.instr->opcode,"RESW")==0) || (strcmp(intr_line.instr->opcode,"RESB")==0)){
			strcpy(obj_code,"");
			strcpy(list_line.obj_code, "");	//Copy it to list_line struct member for write_line_list() operation.
			//@txtrec_end_f = 1;	
		}
		//else neither an opcode nor a directive
		
		//Text record production begins here...
		if((txtrec_ctr < MAXTRLEN) && (strcmp(intr_line.instr->opcode,"RESW") && strcmp(intr_line.instr->opcode,"RESB"))){
			if(reswb_f){	//if RESW or RESB was last found then new text record should begin
				fprintf(obj_prog,"%02X",txtrec_ctr);
				fprintf(obj_prog,"%s\n",text_rec);
				//Reset text_rec string
				strcpy(text_rec,"");
				txtrec_ctr = 0x0;
				//uint16_t temp;
				//sscanf(intr_line.addr,"%X",&temp);
				//hex_addr = temp;
				fprintf(obj_prog,"T^%06X^",intr_line.addr);
				reswb_f = 0;
			}

			//Add obj_code to current text record.(Be careful from adding a '^' with a NULL obj_code)
			if(strcmp(obj_code,"") != 0){
				strcat(text_rec,"^");
				strcat(text_rec,obj_code);
				txtrec_ctr += 0x03;			// (+ 03H) default for simple SIC
			}
		}

		else if(!strcmp(intr_line.instr->opcode,"RESW") || !strcmp(intr_line.instr->opcode,"RESB"))
			reswb_f = 1;	//RESW,RESB was found flag

		else{	//no more place in the current text record.Create a new one
			//Time to flush the generated text_rec string and prepare for a new text record, if any
			//Insert the 'length' and 'object codes' fields of the currently ending text record
			fprintf(obj_prog,"%02X",txtrec_ctr);
			fprintf(obj_prog,"%s\n",text_rec);
			//Reset text_rec string
			strcpy(text_rec,"");
			txtrec_ctr = 0x0;
			//uint16_t temp;
			//sscanf(intr_line.addr,"%X",&temp);
			hex_addr = intr_line.addr.val;
			fprintf(obj_prog,"T^%06X^",hex_addr);
			if(strcmp(obj_code,"") != 0){
				strcat(text_rec,"^");
				strcat(text_rec,obj_code);
				txtrec_ctr += 0x03;
			}
		}
			
		//Write to assembly listing file
		write_line_list(listing, &list_line);
		
		//Read next line
		read_line_intr(intr_file, &intr_line);
		
	}//while(END)
	
	//Insert the left over text record, if any
	if(strcmp(text_rec,"") != 0){
		//Insert the 'length' and 'object codes' fields of the currently ending text record
		fprintf(obj_prog,"%02X",txtrec_ctr);
		fprintf(obj_prog,"%s\n",text_rec);
	}
	//Write last line to assembly listing file	
	write_line_list(listing, &list_line);
	
	//Write the END record
	fprintf(obj_prog,"E^%06X", program_info.start_addr);

	//Close all files
	fclose(listing);
	fclose(intr_file);
	fclose(obj_prog);
}
