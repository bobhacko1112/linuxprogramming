#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#define file_size 3000000000 //roughly 3 billion bytes for large file test.
#define buffer_size 4096 //buffer size for read/write functions.

// cheaty error handle function. it's terrible.
void handleError(char s[150]) {
    printf("ERROR!!! STUFF BROKE BECAUSE: %s \n", s );
    exit(EXIT_SUCCESS); // keyword defined someplace I have no idea.. probably
}

int main(int argc, char *argv[]) {
    int out_file;
    char buffer[300];
    int buf_size = 300;
    int num_read;


    if((out_file = open(argv[1], O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
        handleError((char*)"could not open the file specified");
    }

    if((out_file = open(0, O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
        handleError((char*)"could not open the file specified");
    }
    while ((num_read = read(0, buffer, buf_size)) > 0) {
        if (num_read == -1) {
            handleError((char*)"could not read std out descriptor");
        }
				write(1, buffer, num_read);
				write(out_file, buffer, num_read);
    }

    return 0;
}
