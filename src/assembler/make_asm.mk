#This make file produces the executable assembler program

#Directory for storing the output binary file
BIN_DIR=../../bin/

#Output file
SASM=$(BIN_DIR)sasm

OBJECTS = main.o assembly.o pass1.o pass2.o optab.o _string.o

$(SASM): $(OBJECTS)
	-mkdir $(BIN_DIR)
	gcc -o $(SASM) $(OBJECTS)

main.o: main.c assembly.h
	gcc -c main.c

assembly.o: assembly.c assembly.h flags.h $(SIC_HDR)
	gcc -c assembly.c

pass1.o: pass1.c assembly.h
	gcc -c pass1.c

pass2.o: pass2.c assembly.h
	gcc -c pass2.c

optab.o: optab.c optab.h
	gcc -c optab.c

_string.o: _string.c _string.h
	gcc -c _string.c


clean:
	-rm *.o
	#-rm sasm

#For debugging purpose only
DEBUG_DEPENDENCIES=_string.c optab.c pass1.c pass2.c assembly.c main.c

debug:
	gcc -g $(DEBUG_DEPENDENCIES) -o sasm
