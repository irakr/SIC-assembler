/*
 *	assembly.c
 *
 *	Source file for assembly.h header file.
 */

#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "flags.h"

//TODO...Change all the addresses and integer data variables to standard SIC types.

/*	Search SYMTAB for 'symbol'	*/
int search_symtab(const char *symbol){
	FILE *symtab;
	char sym[10]="",addr[10]="";
	symtab = fopen(".symtab","r");	//SYMTAB file
	if(symtab==NULL){
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return -1;
	}
	if(symbol == (char*)0){	//If empty symbol
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return -1;
	}
	/*	Compare with all symbols	*/
	do{
		fscanf(symtab,"%s",sym);
		fscanf(symtab,"%s",addr);
		if(strcmp(symbol,sym)==0){	//Symbol found in SYMTAB
			fclose(symtab);
			return FOUND;
		}
	} while(!feof(symtab));
	//Symbol invalid
	fclose(symtab);
	return NOTFOUND;
}

/*	Search OPTAB for 'opcode'	*/
int search_optab(const char *opcode){
	FILE *optab;
	char op[10]="",addr[10]="";
	optab = fopen("optab","r");	//OPTAB file
	if(optab==(FILE*)0){
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return -1;
	}
	if(opcode == (char*)0){	//If empty opcode
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return -1;
	}
	/*	Compare with all opcodes	*/
	do{
		fscanf(optab,"%s",op);
		fscanf(optab,"%s",addr);
		if(strcmp(opcode,op)==0){
			fclose(optab);
			return FOUND;
		}
	}while(!feof(optab));
	
	//Invalid opcode
	fclose(optab);
	return NOTFOUND;
}

/*	Get hexcode for 'opcode' from OPTAB	*/
char *get_hexcode(char *opcode){
	FILE *optab=fopen("optab","r");
	char op[10]="",*code=(char*)malloc(sizeof(char)*10);
	if(optab==NULL){
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return (char*)0;
	}
	if(opcode == (char*)0){	//If empty symbol
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return (char*)0;
	}
	/*	Compare with all opcode	*/
	do{
		fscanf(optab,"%s",op);
		fscanf(optab,"%s",code);
		if(strcmp(opcode,op)==0){
			fclose(optab);
			return code;
		}
	}while(!feof(optab));
	fclose(optab);
	return (char*)0;
}

/*	Get address of 'symbol' from SYMTAB	*/
char *get_symbol_addr(char *symbol){
	FILE *symtab=fopen(".symtab","r");
	char sym[10]="",*addr=(char*)malloc(sizeof(char)*10);
	if(symtab==NULL){
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return (char*)0;
	}
	if(symbol == (char*)0){	//If empty symbol
		fprintf(stderr,"(%s : %d)\n", __FILE__, __LINE__);
		return (char*)0;
	}
	/*	Compare with all symbols	*/
	do{
		fscanf(symtab,"%s",sym);
		fscanf(symtab,"%s",addr);
		if(strcmp(symbol,sym)==0){
			fclose(symtab);
			return addr;
		}
	}while(!feof(symtab));
	fclose(symtab);
	return (char*)0;
}

//Insert the length of text record to the ^^ portion
//Divide the text_rec into two parts,insert the length, and then rejoin them. Maybe we can strtok()...but for now let stick with this.
//Eg: T^004000^-^0410024^0010024^184021^2C10021^384006^0C10027^4C0000^00000
void insert_txtlen(char *text_rec, int txtrec_ctr){
	char *temp_txtrec_r,temp_txtrec_l[100]="",temp_ctr[5]="";	//=(char*)calloc(50,sizeof(char));
	strncpy(temp_txtrec_l, text_rec, strcspn(text_rec,"-"));	//Left half
	//printf("%s\n", temp_txtrec_l);
	temp_txtrec_r = strstr(text_rec, "-") + 1;	//Right half
	//printf("%s\n", temp_txtrec_r);
	sprintf(temp_ctr, "%02d", txtrec_ctr);
	//printf("%s\n", temp_ctr);
	strcat(temp_txtrec_l, temp_ctr);
	//printf("%s\n", temp_txtrec_l);
	strcat(temp_txtrec_l, temp_txtrec_r);
	//printf("%s\n", temp_txtrec_l);
	//strcpy(text_rec, "");
	strcpy(text_rec, temp_txtrec_l);
	//printf("%s\n", text_rec);
	//@return text_rec;			
}

/*	This function reads an instruction from the input file	*/
int read_line_src(FILE *f, SIC_Source_line *buff){
	int bytes = 0,t,temp;	//Keep track of the no. of characters being read from a line of the i/p file. We can use this data later...
	bytes += ((t=fscanf(f,"%s",buff->label))==EOF) ? 0 : t;
	bytes += ((t=fscanf(f,"%s",buff->opcode))==EOF) ? 0 : t;
	bytes += ((t=fscanf(f,"%s",buff->operand))==EOF) ? 0 : t;
	return bytes;	//Returning 0 probably means EOF has reached or no instructions were provided.
}

/*	This function writes the examined instruction line to the intermediate file along with memory locations. */
int write_line_intr(FILE *f, SIC_Interm_line *buff){
	int bytes=0;
	bytes += fprintf(f,"%04X\t",buff->addr);
	bytes += fprintf(f,"%s\t",buff->instr->label);
	bytes += fprintf(f,"%s\t",buff->instr->opcode);
	bytes += fprintf(f,"%s\n",buff->instr->operand);
	fflush(f);
	return bytes;
}

/*	Read Line from intermediate file	*/
int read_line_intr(FILE *f, SIC_Interm_line *buff){
	int bytes = 0,t,temp;
	bytes += ((t=fscanf(f,"%X",&temp))==EOF) ? 0 : t;
	buff->addr.val = temp;
	bytes += ((t=fscanf(f,"%s",buff->instr->label))==EOF) ? 0 : t;
	bytes += ((t=fscanf(f,"%s",buff->instr->opcode))==EOF) ? 0 : t;
	bytes += ((t=fscanf(f,"%s",buff->instr->operand))==EOF) ? 0 : t;
	return bytes;
}

/*	Write Line to listing file	*/
int write_line_list(FILE *f, SIC_Listing_line *buff){
	int bytes = 0;
	bytes += fprintf(f,"%04X\t",buff->instr->addr);
	bytes += fprintf(f,"\t%s",buff->instr->instr->label);
	bytes += fprintf(f,"\t%s",buff->instr->instr->opcode);
	bytes += fprintf(f,"\t%s",buff->instr->instr->operand);
	
	//If opcode is anyone of the following: RESW or RESB or START or END. then don't write opcode to listing file.
	if(!(!strcmp(buff->instr->instr->opcode, "RESW") || !strcmp(buff->instr->instr->opcode, "START") || \
		!strcmp(buff->instr->instr->opcode, "START") ||	!strcmp(buff->instr->instr->opcode, "END")))
		bytes += fprintf(f,"\t%s\n",buff->obj_code);
	else
		fprintf(f,"\n");
	fflush(f);
	return bytes;
}

/*
int main(){
	FILE *input,*symtab,*optab;
	SIC_Source_line src_line;
	int locctr;
	
	input = fopen("sum.sic","r");
	symtab = fopen(".symtab","w");
	optab = fopen("optab","r");
	
	locctr = 0;
	while(1){
		read_line_src(input, &src_line);
		if(strcmp(src_line.label,"-")!=0)
			fprintf(symtab,"%s\t%X\n",src_line.label,locctr);
		fflush(symtab);
		if(feof(input))
			break;
	}
	
	fclose(input);
	fclose(symtab);
	fclose(optab);
	return 0;
}
*/
