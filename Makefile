#This is the main makefile that invokes all other makefiles in every sub-directories.

#Global variables
ASSEMBLER_DIR = modules/assembler/
LOADER_DIR = modules/assembler/
HEADERS_DIR = modules/headers/

#include $(ASSEMBLER_DIR)make_asm.mk
#include $(LOADER_DIR)make_loader.mk

clean:
	-rm -rf *.o
