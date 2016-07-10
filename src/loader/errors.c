/*	This function takes the module name(i.e., the caller) as its parameter and checks for 
 *	the appropriate error to generate reports correspondingly and exit.
 */
#include <stdlib.h>
#include "errors.h"

void check_error(const char *module_name){
	
	//TODO check_error
	if(_errno == NO_ERROR)
		return;
	//..........
	exit(1);
}

