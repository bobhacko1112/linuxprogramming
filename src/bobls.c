#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#define buffer_size 4096 //buffer size for read/write functions.

// cheaty error handle function. it's terrible.
void handleError(char s[150]) {
    printf("ERROR!!! STUFF BROKE BECAUSE: %s \n", s );
    _exit(EXIT_SUCCESS); // keyword defined someplace I have no idea.. probably
}

int main(int argc, char *argv[]) {

	struct dirent* dirinfo;
	DIR* dirt = opendir((argc < 2 ? (char*)"." : argv[1]));
	while((dirinfo = readdir(dirt)) != NULL) {
		printf("%s\n",dirinfo->d_name);
	}

	_exit(EXIT_SUCCESS);
}
