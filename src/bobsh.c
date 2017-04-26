/* * bobshell, a crappy shell that does a thing.
 *
 */
#include "../include/boberr.h"
#include "../include/bobsh.h"

struct Proc {
    unsigned int flags;
    pid_t* pid;
    char** argv;
    int* wait_status;
    char** envp;
}; // end struct def

// main function that runs the things.
int run(struct Proc p, pid_t* pid) {
    *pid = fork();
    int pipefd[2];
    pipe(pipefd);

    if (*pid == -1) {
        return -1; // error!
    }

    if (*pid == 0) { // is child
        printf("I AM A CHILD!\n");

        if (p.flags & redirect_pipe_output) {
            // set output of program to pipefd[1]
            printf("PIPE!");
            close(STDOUT_FILENO);
            dup(pipefd[1]);
            close(pipefd[1]);
            close(pipefd[0]);
        }

        if (p.flags & redirect_pipe_input) {
            // set input of program to pipefd[0]
            printf("!redirect_pipe_input");
            close(STDIN_FILENO);
            dup(pipefd[0]);
            close(pipefd[1]);
            close(pipefd[0]);
        }

        if (p.flags & redirect_output_to_file) {
            // redirect stdout to file
        }

        if (p.flags & redirect_error_to_file) {
            // redirect stderr to file
        }

        if (p.flags & redirect_output_to_file_append) {
            // append stdout to file
        }

        if (p.flags & redirect_error_to_file_append) {
            // append stderr to file
        }

        if (p.flags & redirect_error_to_output) {
            // redirect stderr to stdout
        }

        if (p.flags & redirect_file_to_input) {
            // redirect file to stdin
        }

        if (p.flags & is_builtin) {
            // deal with builtins
        }
/*
        if (execve(p.argv[0], p.argv, p.envp) == -1) {
            return -1; // ERROR!
        }
				*/
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(0);
    // end main if body
    return 0;
} // end of run()


/* dirty helper function... enumerates strings.
 * looks at a string and returns a number
 * representing if it's a normal command
 * or a redirect, etc.
 * 0: normal command
 * -1: on error
 * 1-6 are respectively: | > >> &> 2> 2>>
 * 7+ are builtins respecivly: quit, cd, pwd
 */

int commandType(char* command) {
    if (command == NULL) {
        return -1;
    } else if (!strcmp(command, ("|"))) {
        return 1;
    } else if (!strcmp(command, (">"))) {
        return 2;
    } else if (!strcmp(command, (">>"))) {
        return 3;
    } else if (!strcmp(command, ("&>"))) {
        return 4;
    } else if (!strcmp(command, ("2>"))) {
        return 5;
    } else if (!strcmp(command, ("2>>"))) {
        return 6;
    } else if (!strcmp(command, ("quit"))) {
        return 7;
    } else if (!strcmp(command, ("cd"))) {
        return 8;
    } else if (!strcmp(command, ("pwd"))) {
        return 9;
    } else {
        return 0;
    }
}

/*
 *	Gets the input from the user.
 *	uses the new_argvs 2d cstring array
 *	to make a list of argv's to pass for
 *	the forking of new programs.
 *
 *	returns a size_t representing how many
 *	argv's where created within new_argvs.
 *	(Used to easily traverse the 2d array later.)
 */

size_t parseInput(char* new_argvs[50][50]) {
    char* line = malloc(sizeof(char) * 255);
    char* token;
    size_t size = 1;
    int i = 0;
    int j = 0;
    printf("BOBSHELL> ");
    getline(&line, &size, stdin);
    line[strlen(line)-1] = '\0';

    while ((token = strsep(&line, " "))) {
        if (commandType(token) < 7 && commandType(token) != 0) {
            new_argvs[i][j+1] = NULL;
            new_argvs[++i][(j = 0)] = token;
            new_argvs[i++][1] = NULL;
        } else {
            new_argvs[i][j++] = token;
        }
    }

    new_argvs[++i][0] = NULL;
    free(line);
    return i;
}

// diagnostic function. unused in actual program.
void checkArgvs(char* new_argvs[50][50], size_t size) {
    int i,j;

    for(i =0; i < size; i++) {
        for(j = 0; new_argvs[i][j]; j++) {
            printf("('%s' @[%i][%i])", new_argvs[i][j], i, j);
        }

        printf("\n");
    }
}

int mainloop(char** env) {
    // set up vars
    char* new_argvs[50][50];
    char* fn = NULL;
    char* cwd = malloc(sizeof(char) * 50);
    pid_t new_pid;
    size_t size;
    int i,j, command_type, wait_status;
    int pipefd[2];
    //parse command into new_argv
    size = parseInput(new_argvs);

    for (i = 0; i < size; i++) {
        command_type = commandType(new_argvs[i][0]);

        //builtins
        switch (command_type) {
        case 7:
            //quit
            return 0;
            break;

        case 8:
            command_type++;

            if ( chdir(new_argvs[i][1]) == -1) {
                printf("Error,%s",new_argvs[i][1]);
            }

        case 9:
            //pwd
            getcwd(cwd, 50);
            printf("\nDIR:%s\n", cwd);
            free(cwd);
            return 1;
            break;
        };

        //check to see if need to redirect
        if ((command_type = commandType(new_argvs[i+1][0]))) {
            switch (command_type) {
            case 1:

                //pipe
                if (pipe(pipefd) == -1) {
                    handleError();
                } else {
                    if ((new_pid = fork()) ==-1) {
                        handleError();
                    } else if (new_pid == 0) { // is child
                        dup2(1, pipefd[1]);

                        if (execve(new_argvs[i][0], new_argvs[i], env) == -1) {
                            handleError();
                        }
                    } else { // is parent
                        i++;
                        dup2(0, pipefd[0]);
                        waitpid(new_pid, &wait_status,0);
                        return 1;
                        continue;
                    }
                }

                break;

            case 2:
                // > stdout to file
                break;

            case 3:
                // >> stdout append to file
                break;

            case 4:
                // &> stdout and stderr to file
                break;

            case 5:
                // 2> stderr to file
                break;

            case 6:
                // 2>> stderr append to file
                break;
            };
        }

        //fork and execute.
        if ((new_pid = fork()) == -1) {
            handleError();
        } else if (new_pid == 0) { // is child
            if (execve(new_argvs[i][0], new_argvs[i], env) == -1) {
                handleError();
            }
        } else {
            waitpid(new_pid,&wait_status,0);
        }
    }

    return 1;
}

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;
    int i,j, command_type, wait_status;
    char* new_argv[3] = {"/bin/echo","tacotest",0};
    char* new_argv2[3] = {"/usr/bin/tee", "file",0};
    int pipefd[2];
    pipe(pipefd);
    struct Proc new_process;
    new_process.argv = new_argv;
    new_process.envp = envp;
    new_process.flags = redirect_pipe_output;
    wait(0);
    struct Proc new_process2;
    new_process2.argv = new_argv2;
    new_process2.envp = envp;
    new_process2.flags = redirect_pipe_input | redirect_file_to_input;
    run(new_process, &pid);
    run(new_process2, &pid);
    return 0;
//	while (mainloop(envp)) continue;
}
