/*
 *	This is the full implementation of pass-1 of the SIC assembler
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define WORDSIZE 3
#define SINGLEQUOTE 0x27	//The symbol '

//Number system
typedef enum{DEC,BIN,OCT,HEX} base_t;

int prog_len;	//Program length(To be also used by pass2)

/*	Flags	*/
int dupsym_f, invalid_const_f, invalid_op_f;

/*	Flag resetter	*/
void reset_flags(){
	dupsym_f = invalid_const_f = invalid_op_f = 0;
}

/*	Search for 'symbol' in the SYMTAB	*/
int search_symtab(const char *symbol){
	FILE *symtab;
	char sym[10]="";
	if((symtab=fopen("symtab","r")) == NULL){//SYMTAB not found error check
		perror("fopen");
		return -1;//I think exit(1) would be a better option.
	}
	//printf("\nsearch_symtab(): symtab posn = %ld\n",ftell(symtab));
	rewind(symtab);
	//printf("\nsearch_symtab(): symtab posn = %ld\n",ftell(symtab));
	if(symbol==(char*)0){	//Symbol == NULL error check
		perror("char *symbol is NULL");
		return -1;
	}
	/*	Keep on verifying symbol with symbols in the SYMTAB	*/
	while(fscanf(symtab,"%s",sym) != EOF){
		//printf("\nsearch_symtab(): symtab posn = %ld\n",ftell(symtab));
		//printf("\n%s==%s\n",symbol,sym);
		if(strcmp(symbol,sym)==0){	//symbol found(Duplicate)
			fclose(symtab);
			dupsym_f = 1;
			return -1;
		}
	}
	/* If no duplicacy was found	*/
	fclose(symtab);
	return 0;
}

/*	Search for 'opcode' in the OPTAB	*/
int search_optab(const char *opcode){
	FILE *optab;
	char op[10]="";
	if((optab=fopen("optab","r")) == NULL){//SYMTAB not found error check
		perror("optab");
		return -1;
	}
	//printf("\nsearch_optab(): optab posn = %ld\n",ftell(optab));
	rewind(optab);	//Reset file pointer for security purpose. :D
	if(opcode==(char*)0){	//Symbol == NULL error check
		perror("char *opcode is NULL");
		return -1;
	}
	/*	Keep on verifying symbol with symbols in the SYMTAB	*/
	while(fscanf(optab,"%s",op) != EOF){
		//printf("\nsearch_optab(): optab posn = %ld\n",ftell(optab));
		if(strcmp(opcode,op) == 0)	//Opcode found valid.
			break;
	}
	/* If opcode was invalid	*/
	if(feof(optab)){
		fclose(optab);
		invalid_op_f = 1;
		return -1;
	}
	/*If opcode was found to be valid	*/
	fclose(optab);
	return 0;
}
/*	This function reads an instructin from the input file	*/
int read_line(FILE *src,char *label,char *opcode,char *operand){
	int bytes = 0,t,temp;	//Keep track of the no. of characters being read from a line of the i/p file. We can use this data later...
	bytes += ((t=fscanf(src,"%s",label))==EOF) ? 0 : t;
	bytes += ((t=fscanf(src,"%s",opcode))==EOF) ? 0 : t;
	bytes += ((t=fscanf(src,"%s",operand))==EOF) ? 0 : t;
	return bytes;	//Returning 0 probably means EOF has reached or no instructions were provided.
}
/*	This function writes the examined instruction line to the intermediate file along with memory locations. */
int write_line(FILE *intr_file,const char *label,const char *opcode,const char *operand,int loc){
	int bytes=0;
	bytes += fprintf(intr_file,"%X\t",loc);
	bytes += fprintf(intr_file,"%s\t",label);
	bytes += fprintf(intr_file,"%s\t",opcode);
	bytes += fprintf(intr_file,"%s\n",operand);
	return bytes;
}

//main
int main(int argc, char *argv[]){
	FILE *src,*symtab_wr,*intr_file;
	char *ofile, proglen[20];

	char opcode[10]="",label[10]="",operand[10]="";
	int bytes,start_addr,locctr,inc,const_len;
	if(argc <= 1){
		fprintf(stderr,"\nError: No source filename provided\n");
		exit(EXIT_FAILURE);
	}
	if((src = fopen(argv[1],"r")) == NULL){
		fprintf(stderr,"\nError: No file exists with the name '%s'\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	if((symtab_wr = fopen("symtab","w")) == NULL){
		fprintf(stderr,"\nError: Could not open SYMTAB.\n");
		exit(EXIT_FAILURE);
	}
	
	//Generating output(intermediate) filename
	ofile = (char*)calloc(strlen(argv[1])-3,sizeof(char));
	printf("%s\n",strncpy(ofile,argv[1],strlen(argv[1])-4));
	if((intr_file = fopen(ofile,"w")) == NULL){
		fprintf(stderr,"\nError: Could not open intermediate file.\n");
		exit(EXIT_FAILURE);
	}
	//printf("\nmain(): symtab posn = %ld\n",ftell(symtab_wr));
	reset_flags();	//Reset all flags first
	/*	Read first input line	*/
	printf("\nReading first input line...\n");

	read_line(src,label,opcode,operand);
	//printf("\n%d characters read from the line.\n");
	
	/* opcode=="START"	*/
	if(strcmp(opcode,"START")==0){
		sscanf(operand,"%x",&start_addr);
		locctr = start_addr;
		write_line(intr_file,label,opcode,operand,locctr);	//Define these two functions
		read_line(src,label,opcode,operand);	//Second line onwards
	}
	else	locctr = 0;

	/*	Read rest of the lines	*/	
	while(strcmp(opcode,"END")!=0){
		if(strcmp(label,".") == 0){	//If comment line
			read_line(src,label,opcode,operand);
			continue;
		}

		inc = 0;	//increment value for locctr
		if(strcmp(label,"-")!=0){//If there is something in the label field
			if(search_symtab(label) == -1)	//Symbol already defined
				fprintf(stderr,"\nError: Symbol '%s' is already defined.\n",label);
			else{	//Insert the (symbol,locctr) pair into the SYMTAB
				fprintf(symtab_wr,"%s\t%X\n",label,locctr);
				//printf("\nmain(): symtab posn = %ld\n",ftell(symtab_wr));
				printf("\nSymbol inserted to SYMTAB.\n");
			}//if search_symtab()
			fflush(symtab_wr);
		}//if label is not empty(Not an error)
		else	fprintf(stderr,"\nEmpty label.\n");
			/*	Search OPTAB for opcode	*/
		if(search_optab(opcode) == 0){//Search OPTAB for opcode
			inc += WORDSIZE;
			printf("\nOpcode is valid.\n");
		}
		/* Check the opcode field with various assembler directives	*/
		else if(strcmp(opcode,"WORD")==0)
			inc = WORDSIZE;
		else if(strcmp(opcode,"RESW")==0)
			inc = (WORDSIZE * atoi(operand));
		else if(strcmp(opcode,"RESB")==0)
			inc = atoi(operand);
		else if(strcmp(opcode,"BYTE")==0){
			if(operand[0] == 'C')//For character constants
				const_len = strlen(index(operand,SINGLEQUOTE)) - 2;		//Excluding ' from the string 'EOF'
			else if(operand[0] == 'X')//For hexadecimal constants
				const_len = 1;
			else{
				invalid_const_f = 1;	//Invalid constant type flag
				fprintf(stderr,"\nError: Invalid constant type '%s'\n",operand);
			}
			inc = const_len;
		}
		else{
			invalid_op_f = 1;	//Invalid opcode flag
			fprintf(stderr,"\nError: Invalid opcode.\tAt label: %s\n",label);
			exit(1);
		}//if(search OPTAB)
		//printf("-----------------------------------------------\n");
		write_line(intr_file,label,opcode,operand,locctr);//Write line to intermediate file
		locctr += inc;	//Update locctr now
		read_line(src,label,opcode,operand);	//Read next input line
		reset_flags();	//Reset all flags
	}//while()
	write_line(intr_file,label,opcode,operand,locctr);	//Write last line
	prog_len = (locctr - start_addr) + 1;
	printf("\nLength of program = %d(DEC) or %X(HEX)\n",prog_len,prog_len);
	fclose(src);
	fclose(symtab_wr);
	fclose(intr_file);
	sprintf(proglen,"%d",prog_len);	//Program length to sent to pass2 using execl will require the data type to be a string
	
	/*	Execute pass2	*/
	switch (fork()){
		case -1:	perror("fork");
					break;
		case 0:		//Child process
					printf("\n\tInitiating pass2...\n");
					if(execl("pass2","pass2",ofile,proglen,(char*)0) == -1)
						perror("execl");
					exit(0);
		default:	//Parent process
					wait(NULL);
	}
	return 0;
}
