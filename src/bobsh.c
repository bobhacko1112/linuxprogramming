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
    printf("ERROR %i: %s:\n %s",errno, sys_errlist[errno], s);
    _exit(EXIT_SUCCESS); // keyword defined someplace I have no idea.. probably
}
int mainloop(char** env) {
    // set up vars
    pid_t new_pid;
    int wait_status;
    char* new_argv[50];
    char* fn = NULL;
    char* line = malloc(sizeof(char) * 255);
    size_t size = 1;
    char* token;
    int i = 0;
    //prompt user for command
    printf("%s",(char*)"\nBOBSHELL>");
    getline(&line, &size, stdin);

    //parse command into new_argv
    while ((token = strsep(&line, " ")) != NULL) {
        new_argv[i++] = token;
    }

    new_argv[i--] = NULL; // make sure we cap off the new argv. and set i to be the last legal element.
		//now terminate the last string with the correct \0 because strsep doesn't do it for the last element. how lame.
		int j = 0;
		while (new_argv[i][j] != '\0') {
			if (new_argv[i][j] == '\n') {
				new_argv[i][j] = '\0';
				break;
			}
			j++;
		}

		// Check for 'Builtin' commands.
    if (strcmp(new_argv[0], (char*)"exit\0") == 0) {
        free(line);
        return 0;
    } else if (strcmp(new_argv[0], (char*)"pwd\0") == 0) {
        free(line);
        printf("%s",(char*)get_current_dir_name());
        return 1;
    } else if (strcmp(new_argv[0], (char*)"cd\0") == 0) {
			if (chdir(new_argv[1]) == -1) {
				printf("INVALID DIR");
				free(line);
				return 1;
			}
			printf("\nPath is now: %s\n", (char*)get_current_dir_name());
			return 1;
		}

    //fork and execute new process.
    if ((new_pid = fork()) == -1) {
        handleError((char*)"could not fork\0");
    } else if (new_pid == 0) { // is child

        if (execve(new_argv[0], new_argv, env) == -1) {
            handleError((char*)"could not execute new process\0");
        }

    } else {
        waitpid(new_pid,&wait_status,0);
        free(line);
        printf("\nControl returned to parent process.");
        return 1;
    }

    free(line);
    return 1;
}

int main(int argc, char *argv[], char *envp[]) {
    while(mainloop(envp) == 1) {
        continue;
    }

    _exit(EXIT_SUCCESS);
}
