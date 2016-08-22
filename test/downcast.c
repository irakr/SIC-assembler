#include <stdio.h>
#include <stdlib.h>
int main(){
	int a;
	u_char c[2]="";
	printf("Enter a number: ");
	scanf("%i",&a);
	*c = a;
	*(c+1) = a >> 8;
	printf("int a = %d\tchar c[0] = %d\tc[1] = %d\n",a,c[0],c[1]);
	return 0;
}
