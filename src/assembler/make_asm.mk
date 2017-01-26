#This make file produces the executable assembler program

#Directory for storing the output binary file
BIN_DIR=../../bin/

#Output file
SASM=$(BIN_DIR)sasm

#Directories to be searched by the compiler
INCLUDE_DIRS=-I../headers/

OBJECTS = main.o assembly.o pass1.o pass2.o optab.o _string.o

$(SASM): $(OBJECTS)
	-mkdir $(BIN_DIR)
	gcc $(INCLUDE_DIRS) -o $(SASM) $(OBJECTS)

main.o: main.c assembly.h
	gcc $(INCLUDE_DIRS) -c main.c

assembly.o: assembly.c assembly.h flags.h $(SIC_HDR)
	gcc $(INCLUDE_DIRS) -c assembly.c

pass1.o: pass1.c assembly.h
	gcc $(INCLUDE_DIRS) -c pass1.c

pass2.o: pass2.c assembly.h
	gcc $(INCLUDE_DIRS) -c pass2.c

optab.o: optab.c optab.h
	gcc $(INCLUDE_DIRS) -c optab.c

_string.o: _string.c _string.h
	gcc $(INCLUDE_DIRS) -c _string.c


clean:
	-rm *.o
	#-rm sasm

#For debugging purpose only
DEBUG_DEPENDENCIES=_string.c optab.c pass1.c pass2.c assembly.c main.c

debug:
	gcc $(INCLUDE_DIRS) -g $(DEBUG_DEPENDENCIES) -o sasm
