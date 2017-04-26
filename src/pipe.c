#include <unistd.h>
#include <stdlib.h>
int main(int argc, char** argv) {
    int des_p[2];

    if(pipe(des_p) == -1) {
        exit(1);
    }

    if(fork() == 0) {
        close(STDOUT_FILENO);
        dup(des_p[1]);
        close(des_p[0]);
        close(des_p[1]);
        const char* prog1[] = { "./scat", "file", 0};
        execvp(prog1[0], prog1);
    }

    wait(0);

    if(fork() == 0) {
        close(STDIN_FILENO);
        dup(des_p[0]);
        close(des_p[1]);
        close(des_p[0]);
        const char* prog2[] = { "./stee", "lol", 0};
        execvp(prog2[0], prog2);
    }

    close(des_p[0]);
    close(des_p[1]);
    wait(0);
    return 0;
}
