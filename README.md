# SIC-assembler
An assembler emulator for a hypothetical machine called SIC(Simplified Instructional Computer)

# How to build
Using assembler:
- Go to the directory modules/assembler/ and run the command:
	$ make --file=make_asm.mk
- After the build operation, the executable binary file for the assembler module will be available in the directory 'SIC-assembler/bin/'
  which is again inside a directory where you have downloaded the package.
- Although, the executable must have been already built. So you may not have to perform the above steps if there is a file 'sasm' in
  the bin/ directory.


# How to use
- Write any program in SIC(not SIC/XE) assembly language first and save it with an extension 'xyz.sic'. Eg: sum.sic
 [NOTE: You have to use a '-' character to indicate an empty field of an instruction. Fields are LABEL, OPCODE, OPERAND.]
- If you didn't add the SIC-assembler/bin/ directory in the PATH environment variable then you have to use the full pathname of the assembler executable file(sasm).
- But if you don't want these troubles then you can simply add the SIC-assembler/bin directory to the PATH environment variable.
- After assembling, the assembler will produce three important files that we are interested in:
  1) Symbol table named .symtab
  2) Intermediate file named .intermediate
  3) Listing file named .listing
- In linux, the filenames starting with a period(.) is a hidden file. So the above 3 files may not be visible in the GUI-based
  file-manager like 'nautilus'(if appropriate configurations are not done). So, I recommend using CLI to view them. You can simply
  use the ll command to list all files in the directory(including hidden files). You will be able to see those 3 files and check
  them out.
#
	THAT's IT FOR NOW !!!!!
