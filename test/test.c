#include <stdio.h>
int search_symtab(const char *symbol){
	FILE *symtab;
	char sym[10]="",addr[10]="";
	symtab = fopen("symtab.txt","r");	//SYMTAB file
	if(symtab==NULL)
		return -1;
	rewind(symtab);
	if(symbol == (char*)0){	//If empty symbol
		return -1;
	}
	/*	Compare with all symbols	*/
	do{
		fscanf(symtab,"%s",sym);
		fscanf(symtab,"%s",addr);
		if(strcmp(symbol,sym)==0){
			fclose(symtab);
			return 0;
		}
	}while(!feof(symtab));
	//No duplicacy
	fclose(symtab);
	return 0;	//Success
}
int search_optab(const char *opcode){
	FILE *optab;
	char op[10]="",addr[10]="";
	optab = fopen("optab.txt","r");	//OPTAB file
	if(optab==(FILE*)0)
		return -1;
	rewind(optab);
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
int main(){
	printf("%d",search_symtab("TOTAL"));
	printf("%d",search_optab("ADD"));
	return 0;
}
