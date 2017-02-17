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

// this function hawks up a buffer of s size  (in bytes) of goo. to b buffer
void alphabetPuker(int s, char* b) {
    int i;

    for (i = 0; i < s; i++) {
        b[i] = 'a' + (i % 26);
    }

    b[s-1] = '\0'; //slap an end-line on this sucker.
}

int main(int argc, char *argv[]) {
    // declare variables.
    int file_descriptor_output;
    int file_descriptor_input;
    int flags = O_CREAT | O_WRONLY |
                O_TRUNC; // will fail if it must create a file with O_CREAT flag. 'cause we're copying here.
    mode_t permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH |
                         S_IWOTH;      /* rw-rw-rw- */
    int buffer_length; // will use to keep track of buffer length (current size).
    char buffer[buffer_size];

    // check for arguments.
    if (argc < 3) {
        handleError((char*)
                    "Incorrect usage! I take 2 arguments, they are filenames.");
    }

    // open the file from argument 1, read only.
    file_descriptor_input = open(argv[1], O_RDONLY);

    if (file_descriptor_input == -1) {
        handleError((char*)"First file open call messed up");
    }

    // now open the new file we want to copy to.
    file_descriptor_output = open(argv[2], flags, permissions);

    if (file_descriptor_output == -1) {
        handleError((char*)"First output file open call messed up");
    }

    // read input descriptor until empty and put it in output descriptor.
    while((buffer_length = read(file_descriptor_input, buffer,
                                buffer_size)) > 0) {
        if (buffer_length == -1) {
            handleError((char*)"Error reading first file.");
        }

        if ( write(file_descriptor_output, buffer, buffer_length) == -1) {
            handleError((char*)"Error writing to output file!");
        }
    }

    // close that stuff! easy!
    close(file_descriptor_input);
    close(file_descriptor_output);
    printf("Successful operation!\n");
    return 0;
}
