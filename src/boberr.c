#include "../include/boberr.h"

void handleError(char s[150]) {
    printf("ERROR %i: %s",errno, sys_errlist[errno]);
	 
		_exit(EXIT_SUCCESS); // keyword defined someplace I have no idea.. probably
}
