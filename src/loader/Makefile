#	A BASIC MAKEFILE
#This is a test makefile. The standard name convention for makefiles 
#is 'Makefile', if you run the make command as 'make' only.
#The make command format is:
#	$ make [OPTION]... [TARGET]...

#The format of a makefile is:
#<target>:	<dependency>[...,<dependency>]
#	<command>
#
#The <target> may be anything, such as an object file,executable file,etc.
#The <dependency>(or a list of dependencies) are those that will produce the
#mentioned target. For eg: In this Makefile, see the second portion(loader:..)
#Here, 'loader' is the target which is an executable file. This file has the dependencies 'loader.o', 'errors.o' and 'packing.o'. This means that the executable
#file 'loader' will be created by compiling and linking these object(.o) files.
#And the command that will do this is given below, i.e.,
#	gcc loader.o errors.o packing -o loader
#
#Again the dependencies 'loader.o', 'errors.o' and 'packing.o' also have their
#own depencies and commands that will generate them.
#
##############################################################################
#Now when you run the command 'make' in the terminal, the flow of execution
#will be as follows:
# 1) If no target is specified explicitly, the make utility will automatically
#    resolve the first target in the Makefile. In our case, the first target
#    is 'all'.
# 2) When resolving a target, it will check what are the dependencies and
#    whether they are themselves a target or not. If so, those dependencies
#    are first resolved in the order they are written.
# 3) In this way, the make utility will recursively resolve al dependencies
#    encountered and finally resolve the first one.
# 4) When resolving targets, the commands are the actual actions that determines
#    what action to perform in order to resolve them.
##############################################################################
#If you want to build only a particular target out of all target in
#a Makefile, you can use the command:
#	$ make <target>
#	For eg: $ make clean
#	will only build the target 'clean'. Since it has no dependencies, only
#	the command 'rm -rf *o loader' will be executed.
#############################################################################
#We can also use variables in makefiles.
#For eg:
#If you want to store the command 'gcc'(or say '-c -Wall' flags) in a variable,
#you can do so as shown below:
#CC=gcc		;Here CC is the variable name and gcc is the value
#CFLAGS=-c -Wall	;Similar '' '' '' ''
#We can use these variables as:
#	$(CC) $(CFLAGS) errors.c
#This is equivalent to writing:
#	gcc -c -Wall errors.c
#We already know the advantages of using a variable instead of literal values.:D
############################################################################

CC=gcc
CFLAGS=-c -Wall

all:	loader

loader:	loader.o errors.o packing.o
	gcc loader.o errors.o packing.o -o loader

loader.o:	loader.c
	gcc -c loader.c

#Usage of the variables are shown here only. Just for tutorial purpose.
errors.o:	errors.c
	$(CC) $(CFLAGS) errors.c

packing.o:	packing.c
	gcc -c packing.c

clean:
	rm -rf *o loader
