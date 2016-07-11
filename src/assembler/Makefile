#This make file produces the executable assembler program
OBJECTS = main.o assembly.o pass1.o pass2.o
SIC_HDR = ../headers/sic.h

sasm: $(OBJECTS)
	gcc -o sasm $(OBJECTS)

main.o: main.c assembly.h
	gcc -c main.c

assembly.o: assembly.c assembly.h flags.h $(SIC_HDR)
	gcc -c assembly.c

pass1.o: pass1.c assembly.h
	gcc -c pass1.c

pass2.o: pass2.c assembly.h
	gcc -c pass2.c

clean:
	rm *.o
	rm sasm
	rm *~
	rm .[a-zA-Z]
