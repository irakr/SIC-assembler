#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORDSIZE 3		//Default word size for simple SIC
#define MAXTRLEN 0x1E	//Maximum bytes for a single Text Record

#define SINGLEQUOTE 0x27	//ASCII Hexcode for singlequote(') character 

/*	Flags	*/
int sym_f=0;

/*	Search SYMTAB	*/
int search_symtab(const char *symbol){
	FILE *symtab;
	char sym[10]="",addr[10]="";
	symtab = fopen("symtab","r");	//SYMTAB file
	if(symtab==NULL)
		return -1;
	if(symbol == (char*)0){	//If empty symbol
		return -1;
	}
	/*	Compare with all symbols	*/
	do{
		fscanf(symtab,"%s",sym);
		fscanf(symtab,"%s",addr);
		if(strcmp(symbol,sym)==0){	//Symbol found in SYMTAB
			fclose(symtab);
			return 0;
		}
	}while(!feof(symtab));
	//Symbol invalid
	fclose(symtab);
	return -1;
}

/*	Search OPTAB	*/
int search_optab(const char *opcode){
	FILE *optab;
	char op[10]="",addr[10]="";
	optab = fopen("/home/irakr/Programs/SP/SIC_assembler/resources/optab","r");	//OPTAB file
	if(optab==(FILE*)0)
		return -1;
	if(opcode == (char*)0){	//If empty opcode
		return -1;
	}
	/*	Compare with all opcodes	*/
	do{
		fscanf(optab,"%s",op);
		fscanf(optab,"%s",addr);
		if(strcmp(opcode,op)==0){
			fclose(optab);
			return 0;
		}
	}while(!feof(optab));
	
	//Invalid opcode
	fclose(optab);
	return -1;
}
	

/*	Read Line	*/
int read_line(FILE *f,char *loc,char *label,char *opcode,char *operand){
	int bytes = 0,t;
	bytes += ((t=fscanf(f,"%s",loc))==EOF) ? 0 : t;
	bytes += ((t=fscanf(f,"%s",label))==EOF) ? 0 : t;
	bytes += ((t=fscanf(f,"%s",opcode))==EOF) ? 0 : t;
	bytes += ((t=fscanf(f,"%s",operand))==EOF) ? 0 : t;
	return bytes;
}
/*	Write Line	*/
int write_line(FILE *f, char *loc, char *label, char *opcode, char *operand, char *obj_code){
	int bytes = 0;
	bytes += fprintf(f,"%s\t",loc);
	bytes += fprintf(f,"%s\t",label);
	bytes += fprintf(f,"%s\t",opcode);
	bytes += fprintf(f,"%s\t",operand);
	bytes += fprintf(f,"%s\n",obj_code);
	return bytes;
}
char *get_symbol_addr(char *symbol){
	FILE *symtab=fopen("symtab","r");
	char sym[10]="",*addr=(char*)malloc(sizeof(char)*10);
	if(symtab==NULL)
		return (char*)0;
	if(symbol == (char*)0){	//If empty symbol
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
char *get_hexcode(char *opcode){
	FILE *optab=fopen("/home/irakr/Programs/SP/SIC_assembler/resources/optab","r");
	char op[10]="",*code=(char*)malloc(sizeof(char)*10);
	if(optab==NULL)
		return (char*)0;
	if(opcode == (char*)0){	//If empty symbol
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

/*MAIN*/
int main(int argc, char *argv[]) {
	FILE *intr_file,*assembly,*obj_prog;
	char loc[10]="",label[10]="",opcode[10]="",operand[10]="",symval[10]="",obj_code[10]="";
	char *objprog_name,text_rec[81]="",prog_name[10]="";	//81bytes is enough to hold a text record. 
	int x = 0x1000,prog_len,txtrec_ctr = 0x0,hex_addr,start_addr;	//program length is just taken as fixed for now.
	int reswb_f = 0;	//RESW or RESB found flag
	
	/*Open all files
	intr_file = fopen("sum","r");	//Intermediate file
	assembly = fopen("a_list","w");	//Assembly listing file
	obj_prog = fopen("sum_obj","w");	//Object file
	*/
	
	//pwd
	printf("Current working directory: %s\n",get_current_dir_name());
	prog_len = atoi(argv[2]);
	//printf("???? %d\n",prog_len);
	if(argc <= 1){
		fprintf(stderr,"\nError: No source filename provided\n");
		exit(EXIT_FAILURE);
	}
	if((intr_file = fopen(argv[1],"r")) == NULL){
		fprintf(stderr,"\nError: No file exists with the name '%s'\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	if((assembly = fopen("listing","w")) == NULL){	//Assembly listing file
		fprintf(stderr,"Error: Cannot open file : listing\n");
		exit(EXIT_FAILURE);
	}
	objprog_name = (char*)calloc(strlen(argv[1])+5,sizeof(char));
	strcpy(objprog_name,argv[1]);
	strcat(objprog_name,".obj");
	//Object program file
	if((obj_prog = fopen(objprog_name,"w")) == NULL){
		fprintf(stderr,"\nError: Could not open file '%s'\n",objprog_name);
		exit(1);
	}
	//reset_flags();	//Reset all flags
	
	//printf("Reading first line...\n");
	read_line(intr_file,loc,label,opcode,operand);
	
	//First startement of the source program
	if(strcmp(opcode,"START")==0){
		write_line(assembly,loc,label,opcode,operand,"");

		//Create the header record components
		sscanf(operand,"%x",&hex_addr);
		strcpy(prog_name,label);
		start_addr = hex_addr;
		//Write object program header record
		fprintf(obj_prog, "H^%-6s^%06X^%06X\n",prog_name,hex_addr,prog_len);

		read_line(intr_file, loc, label, opcode, operand);
	}
	
	//Initialize first text record
	//@sprintf(text_rec,"T^%06d^-",atoi(loc));
	sscanf(loc,"%x",&hex_addr);
	fprintf(obj_prog,"T^%06X^",hex_addr);
	//printf("%s\t%d\n",text_rec,strlen(text_rec));
	//Scan till 'END' statement or End of file
	while((strcmp(opcode,"END")!=0) && !feof(intr_file)){
		//If comment line
		if(strcmp(label,".") == 0){
			read_line(intr_file,loc,label,opcode,operand);
			continue;
		}
		//Else parse line....
		if(search_optab(opcode) == 0){	//Opcode found
			if(strcmp(operand,"-") != 0){//Not an empty label field
				if(search_symtab(operand)==0)//Symbol found
					//@strcpy(symval,get_symbol_addr(operand));
					sscanf(get_symbol_addr(operand),"%x",&hex_addr);
				else if(strstr(operand,",X") != (char*)0){	//For indexed addressing, add value of X to address
					char temp_sym[10]="";
					int temp_addr;
					strncpy(temp_sym,operand,strcspn(operand,",X"));//Extract the symbol(excluding ',X') from the operand field
					sscanf(get_symbol_addr(temp_sym),"%x",&hex_addr);
					hex_addr += x;//Add hex_aadr+indexregister. Computed as integers
					//@sprintf(symval,"%d",temp_addr);	//Convert the integer address back to string and store in symval
				}
				else{
					//@strcpy(symval,"0000");
					sscanf("0","%x",&hex_addr);
					sym_f = 1;	//Invalid symbol flag
				}
			}//operand "-"
			else	
				//@strcpy(symval,"0000");
				sscanf("0","%x",&hex_addr);
			//Make the complete object code
			sprintf(symval,"%04X",hex_addr);
			//printf("symval: %s\thex_addr: %d\n",symval,hex_addr);
			strcpy(obj_code,get_hexcode(opcode));	// 04
			//@strcat(obj_code,symval);
			strcat(obj_code,symval);	//04400C
			
		}//if opcode found
		else if(strcmp(opcode,"WORD")==0){
			sscanf(operand,"%x",&hex_addr);
			sprintf(obj_code,"%06X",hex_addr);
		}
		else if(strcmp(opcode,"BYTE")==0){
			char temp[10]="";
			if(operand[0] == 'C'){	//Eg: C'EOF'
				char *ptr;
				ptr = strstr(operand,"'") + 1;
				if(strcmp(ptr,"EOF'") == 0)
					strcpy(temp,"454F46");
				//Any other character constants can be added here.....
				else{
					fprintf(stderr,"\nError: Invalid constand type\tAt label: %s\n",label);
					exit(1);
				}
			}
			else if(operand[0] == 'X'){	//Eg: X'F1'
				char *ptr;
				short int i=0;
				ptr = strstr(operand,"'") + 1;
				while(*ptr != SINGLEQUOTE){
					temp[i] = *ptr;
					ptr++; i++;
				}
			}
			sscanf(temp,"%x",&hex_addr);
			sprintf(obj_code,"%02X",hex_addr);
		}
		//For RESW and RESB
		else if((strcmp(opcode,"RESW")==0) || (strcmp(opcode,"RESB")==0)){
			strcpy(obj_code,"");
			//@txtrec_end_f = 1;	
		}
		//else unknown opcode or directive
		
		//Text record jobs begin...
		if((txtrec_ctr < MAXTRLEN) && (strcmp(opcode,"RESW") && strcmp(opcode,"RESB"))){
			if(reswb_f){	//if RESW or RESB was last found then new text record should begin
				fprintf(obj_prog,"%02X",txtrec_ctr);
				fprintf(obj_prog,"%s\n",text_rec);
				//Reset text_rec string
				strcpy(text_rec,"");
				txtrec_ctr = 0x0;
				sscanf(loc,"%x",&hex_addr);
				fprintf(obj_prog,"T^%06X^",hex_addr);
				reswb_f = 0;
			}

			//Add obj_code to current text record.(Be careful from adding a '^' with a NULL obj_code)
			if(strcmp(obj_code,"") != 0){
				strcat(text_rec,"^");
				strcat(text_rec,obj_code);
				txtrec_ctr += 0x03;			// (+ 03H) default for simple SIC
			}
		}

		else if(!strcmp(opcode,"RESW") || !strcmp(opcode,"RESB"))
			reswb_f = 1;	//RESW,RESB was found flag

		else{	//no more place in the current text record.Create a new one
			//Time to flush the generated text_rec string and prepare for a new text record, if any
			//Insert the 'length' and 'object codes' fields of the currently ending text record
			fprintf(obj_prog,"%02X",txtrec_ctr);
			fprintf(obj_prog,"%s\n",text_rec);
			//Reset text_rec string
			strcpy(text_rec,"");
			txtrec_ctr = 0x0;
			sscanf(loc,"%x",&hex_addr);
			fprintf(obj_prog,"T^%06X^",hex_addr);
			if(strcmp(obj_code,"") != 0){
				strcat(text_rec,"^");
				strcat(text_rec,obj_code);
				txtrec_ctr += 0x03;
			}
			//------------- OLD CODE --------------------
			//@int n = strlen(text_rec);
			//@@char *temp_txtrec_r,temp_txtrec_l[100]="",temp_ctr[5]="";	//=(char*)calloc(50,sizeof(char));
			//@text_rec[n-1] = '\0';	//Remove the extra '^' character that is generated 
			//@printf("%s\n", text_rec);
			
			//@insert_txtlen(text_rec,txtrec_ctr);
			//@@strncpy(temp_txtrec_l, text_rec, strcspn(text_rec,"-"));	//Left half
			//@@printf("%s\n", temp_txtrec_l);
			//@@temp_txtrec_r = strstr(text_rec, "-") + 1;	//Right half
			//@@printf("%s\n", temp_txtrec_r);

			//@@sprintf(temp_ctr, "%02d",txtrec_ctr);
			//@@printf("%s\n", temp_ctr);
			//@@strcat(temp_txtrec_l, temp_ctr);
			//@@printf("%s\n", temp_txtrec_l);
			//@@strcat(temp_txtrec_l, temp_txtrec_r);
			//@@printf("%s\n", temp_txtrec_l);
			//@strcpy(text_rec, "");
			//@@strcpy(text_rec, temp_txtrec_l);
			//@@printf("%s\n", text_rec);
			
		}
			
		//Write to assembly listing file
		write_line(assembly,loc,label,opcode,operand,obj_code);
		
		//Read next line
		read_line(intr_file,loc,label,opcode,operand);
		
	}//while(END)
	
	//Insert the left over text record, if any
	if(strcmp(text_rec,"") != 0){
		//Insert the 'length' and 'object codes' fields of the currently ending text record
		fprintf(obj_prog,"%02X",txtrec_ctr);
		fprintf(obj_prog,"%s\n",text_rec);
	}
	//Write last line to assembly listing file	
	write_line(assembly,loc,label,opcode,operand,"");
	
	//Write the END record
	fprintf(obj_prog,"E^%06X",start_addr);

	//Close all files
	fclose(assembly);
	fclose(intr_file);
	fclose(obj_prog);
	return 0;
}
