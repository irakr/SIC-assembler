exec: compile
	echo "------ Executing --------\n"
	./sasm sum.sic

compile: main.c
	echo "Building and compiling source...\n"
	gcc assembly.c pass1.c pass2.c main.c -o sasm
	touch compile

debug: main.c
	echo "Compiling source for debug...\n"
	gcc -g assembly.c pass1.c pass2.c main.c -o sasm
	touch debug
	
