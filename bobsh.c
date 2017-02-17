/*
 * bobshell, a crappy shell that does a thing.
 *
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define buffer_size 4096 //buffer size for read/write functions.

		// cheaty error handle function. it's terrible.
void handleError(char s[150]) {
    printf("ERROR %i: %s\n%s",errno, sys_errlist[errno], s = (char*)"\0");
    _exit(EXIT_SUCCESS); // keyword defined someplace I have no idea.. probably
}

int mainloop(char** env) {
		// set up vars
    pid_t new_pid;
    int wait_status;
    char* cmd[2048];
    char* newarg[] = {(char*)"\0",NULL};
		//prompt user for command
    printf("%s",(char*)"\nBOBSHELL>");
    scanf("%s",(char*)cmd);

		//check for built in function call cases (just exit for now)
    if (strcmp((char*)cmd, (char*)"exit") == 0) {
        return 0;
    }
		//fork and execute new process.
    if ((new_pid = fork()) == -1) {
        handleError((char*)"could not fork");
    } else if (new_pid == 0) { // is child
        if (execve((char*)cmd, newarg, env) == -1) {
					return 0;
        }

    } else { // is parent
        waitpid(new_pid,&wait_status,0);
				return 1;
    }
    return 1;
}

int main(int argc, char *argv[], char *envp[]) {
    while(mainloop(envp) == 1) {
        continue;
    }

    _exit(EXIT_SUCCESS);
}
