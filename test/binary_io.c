#include <stdio.h>

int main(){
	FILE *fp = fopen("sum.obj","r");
	char ch[7]="";
	do{
		if(fread((char*)ch,3,2,fp) <= 0)
			break;
		printf("%s|",ch);
	}while(1);
	return 0;
}
