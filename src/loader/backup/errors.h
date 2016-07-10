/*
 *	errors.h
 *
 *	Error checking utility header file.
 */

#ifndef ERRORS_H_
#define ERRORS_H_

//Macro for setting an error flag
#define SETERROR(ERRNO)	_errno=ERRNO
#define RESETERROR()	_errno=NO_ERROR

//Error flag. Maybe raised by any of the modules in the software.
enum{NO_ERROR=0,ERR_CORRUPT_FILE,ERR_MEMORY_LEAK,ERR_PROGSIZE} _errno;

//Error flags for assembly time checkings.............. Eg: Invalid opcode,duplicate symbol,etc
//.........To be implemented

//Check for error and generate report
void check_error(const char *module_name);

/*	This function takes the module name(i.e., the caller) as its parameter and checks for 
 *	the appropriate error to generate reports correspondingly and exit.
 */
void check_error(const char *module_name){
	
	//TODO check_error
	if(_errno == NO_ERROR)
		return;
	//..........
	exit(1);
}

#endif
