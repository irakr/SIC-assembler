# SIC-assembler
An assembler emulator for a hypothetical machine called SIC(Simplified Instructional Computer)

# How to install
Just execute the 'install.sh' script. When asked for sudo password, enter it.

# How to use
- Write any program in SIC(not SIC/XE) assembly language first and save it with an extension 'xyz.sic'. Eg: sum.sic
- After assembling, the assembler will produce three important files that we are interested in:
  1) Symbol table named '.symtab'
  2) Intermediate file named '.intermediate'
  3) Listing file named '.listing'
  
- In linux, a filename starting with a period(.) is a hidden file. So the above 3 files may not be visible in the GUI-based
  file-manager like 'nautilus'(if appropriate configurations are not done). So, I recommend using CLI to view them. You can simply
  use the 'll' command to list all files in the directory(including hidden files). You will be able to see those 3 files and check
  them out.
#
	THAT's IT FOR NOW !!!!!
	THANKS FOR TRYING IT OUT!
