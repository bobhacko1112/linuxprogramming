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
#define buffer_size 1024 //buffer size for read/write functions.
#define max_arg_count 50 //max amount of arguments that can be passed.

// cheaty error handle function. it's terrible.
void handleError(char s[150]) {
    printf("ERROR %i: %s:\n%s",errno, sys_errlist[errno], s);
    _exit(EXIT_SUCCESS); // keyword defined someplace I have no idea.. probably
}
// create argv** to pass to new process
void readCmd(char** new_argv, char* filename) {
	char* line = malloc(sizeof(char)* 255);
	size_t count;
		getline(&line, &count, stdin);
		printf("%s",(char*)line);
		int i = 0;
		while ((new_argv[i++] = strtok(line, (char*)" ")) != NULL) {

		}


}

int mainloop(char** env) {
    // set up vars
    pid_t new_pid;
    int wait_status;
    char** new_argv = NULL;
		char* fn = NULL;
    //prompt user for command
    printf("%s",(char*)"\nBOBSHELL>");
    //build new argv.
		readCmd(new_argv, fn);
    //check for built in function call cases (just exit for now)
		/*
    if (strcmp(new_argv[0], (char*)"exit\n") == 0) {
        return 0;
    }

    //fork and execute new process.
    if ((new_pid = fork()) == -1) {
        handleError((char*)"could not fork\0");
    } else if (new_pid == 0) { // is child

        if (execve(strcat(new_argv[0], (char*)"\0"), new_argv, env) == -1) {
            handleError((char*)"could not execute new process\0");
        }
    } else {
        waitpid(new_pid,&wait_status,0);
        return 1;
    }
		*/
		return 1;
}

int main(int argc, char *argv[], char *envp[]) {
    while(mainloop(envp) == 1) {
        continue;
    }

    _exit(EXIT_SUCCESS);
}
