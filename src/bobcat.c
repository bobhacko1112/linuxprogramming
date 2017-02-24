/*
 * author: robert hacko, spring 2017
 * description: This program will print on the screen the contents of a file
 * run it without arguments for usage
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#define buffer_size 4096 //buffer size for read/write functions.
#define arg_count 2

// cheaty error handle function. it's terrible.
void handleError(char s[150]) {
    printf("ERROR!!! STUFF BROKE BECAUSE: %s \n", s );
    exit(EXIT_SUCCESS); // keyword defined someplace I have no idea.. probably
}

int main(int argc, char *argv[]) {
    // check for arguments.
    if (argc < arg_count) {
        handleError((char*)
                    "Usage: bobcat [filename] ... {start_pos},{end_pos}");
    }

    // try to open file specified. read only, and only if the file is owned by the caller.
    int fdi; // 'file descriptor input'

    if ((fdi = open(argv[1], O_RDONLY, S_IWUSR)) == -1) {
        handleError((char*)"I failed to open the file specified");
    }

    // print the file out to standard output.
    char buffer[buffer_size];
    int amount_read;

    do {
        if((amount_read = read(fdi, buffer, buffer_size)) == -1) {
            handleError((char*)
                        "Could not read properly from input file descriptor!");
            buffer[amount_read] = '\0'; // make sure printf knows when to stop.
        } else if (amount_read == 0) {
            break;
        }

				//write to standard output's corresponding descriptor. 1 should always be this.
        write(1,buffer, amount_read);
    } while (amount_read > 0);

    if(close(fdi) == -1) {
        handleError((char*)"Failed to close descriptor");
    }

    exit(EXIT_SUCCESS);
}
