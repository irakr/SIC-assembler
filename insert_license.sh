#!/bin/bash
#This shell script inserts the license text at the beginning of each files passed as arguments.

#Storing arguments
args=("$@")

#No. of args
n=${#args[@]}

#Directories
#assembler=src/assembler/
#loader=src/loader/

#Process all arguments one by one
for (( i=0;i<$n;i++ ));
do
	echo "Processing ${args[$i]}"
	cat ${args[$i]} >${args[$i]}.temp
	#echo "/*" >${args[$i]}
	cat LICENSE >${args[$i]}
	#echo "*/" >>${args[$i]}
	cat ${args[$i]}.temp >>${args[$i]}
	rm ${args[$i]}.temp
done 
