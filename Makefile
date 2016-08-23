#This is the main makefile that invokes all other makefiles in every sub-directories.

#Global variables
ASSEMBLER_DIR = src/assembler/
LOADER_DIR = src/assembler/
HEADERS_DIR = src/

#include $(ASSEMBLER_DIR)make_asm.mk
#include $(LOADER_DIR)make_loader.mk

clean:
	-rm -rf *.o
