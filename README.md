# SIC-assembler
An assembler emulator for a hypothetical machine called SIC(Simplified Instructional Computer)

#TODOs:
- Currently only the assembler module is implemented. The loader and runtime modules are not yet implemented.
- SIC/XE features should also be included later.

# How to install
Just execute the 'install.sh' script. When asked for sudo password, enter it.

# How to use
- Write any program in SIC(not SIC/XE) assembly language first and save it with an extension 'xyz.sic'. Eg: sum.sic
- After assembling, the assembler will produce three important files that we are interested in:
  - Symbol table named '.symtab'
  - Intermediate file named '.intermediate'
  - Listing file named '.listing'
  
- Note that the names of the three files mentioned above start with a period(.) character which means they are hidden files.
#
	THAT's IT FOR NOW !!!!!
	THANKS FOR TRYING IT OUT!
