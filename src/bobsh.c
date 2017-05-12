/* * bobshell, a crappy shell that does a thing.
 *
 */
#include "../include/boberr.h"
#include "../include/bobsh.h"


// runs a single process from Proc p.
int run(struct Proc p, pid_t* pid, int pipefd[]) {
    *pid = fork();

    if (*pid == -1) {
        return -1; // error!
    }

    if (*pid == 0) { // is child
        printf("I AM A CHILD!\n");

        if (p.flags & redirect_pipe_output) {
            // set output of program to pipefd[1]
            close(STDOUT_FILENO);
            dup(pipefd[1]);
            close(pipefd[0]);
            close(pipefd[1]);
        }

        if (p.flags & redirect_pipe_input) {
            // set input of program to pipefd[0]
            close(STDIN_FILENO);
            dup(pipefd[0]);
            close(pipefd[1]);
            close(pipefd[0]);
        }

        if (p.flags & redirect_output_to_file) {
            // redirect stdout to file
            close(STDOUT_FILENO);
            open(p.filename,O_WRONLY | O_CREAT);
        }

        if (p.flags & redirect_error_to_file) {
            // redirect stderr to file
            close(STDERR_FILENO);
            open(p.filename,O_WRONLY | O_CREAT);
        }

        if (p.flags & redirect_output_to_file_append) {
            // append stdout to file
            close(STDOUT_FILENO);
            open(p.filename, O_APPEND | O_WRONLY | O_CREAT);
        }

        if (p.flags & redirect_error_to_file_append) {
            // append stderr to file
            close(STDERR_FILENO);
            open(p.filename, O_APPEND | O_WRONLY | O_CREAT);
        }

        if (p.flags & redirect_error_to_output) {
            // redirect stderr to stdout
            close(STDERR_FILENO);
            dup(1);
        }

        if (p.flags & redirect_file_to_input) {
            // redirect file to stdin
            close(STDIN_FILENO);
            open(p.filename,O_RDONLY);
        }

        if (p.flags & is_builtin) {
            // deal with builtins
            switch (commandType(p.argv[0])) {
            case 8: // quit
                exit(0);
                break;

            case 9: // cd
                break;

            case 10: //pwd
                break;
            };
        }

        if (execve(p.argv[0], p.argv, p.envp) == -1) {
            return -1; // ERROR!
        }
    }

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
 */

int commandType(char* command) {
    if (command == NULL) {
        return -1;
    } else if (!strcmp(command, ("|"))) {
        return 1;
    } else if (!strcmp(command, (">")) | !strcmp(command, "1>")) {
        return 2;
    } else if (!strcmp(command, (">>")) | !strcmp(command, "1>>")) {
        return 3;
    } else if (!strcmp(command, "&>") | !strcmp(command, "2>")) {
        return 4;
    } else if (!strcmp(command, ("&>>")) | !strcmp(command, "2>>")) {
        return 5;
    } else if (!strcmp(command,"&")) {
        return 6;
    } else if (!strcmp(command,"<")) {
        return 7;
    } else if (!strcmp(command, ("quit"))) {
        return 8;
    } else if (!strcmp(command, ("cd"))) {
        return 9;
    } else if (!strcmp(command, ("pwd"))) {
        return 10;
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

size_t parseInput(struct Proc* proclist) {
    char* line = malloc(sizeof(char) * 255);
    char* cmd_list[50];
    char* token;
    /* int i is iterator for new_argv
     * int j is iterator for proclist
     * int k is iterator for proclist.argv
     */
    int i,j,k, cmdlist_size;
    size_t s;
    printf("BOBSHELL> ");
    getline(&line, &s, stdin);
    line[strlen(line)-1] = '\0';
    cmdlist_size = 0;
    size_t proclist_size = 0;

    // put input into array of words.
    while ((token = strsep(&line," "))) {
        cmd_list[cmdlist_size++] = strdup(token);
				printf("\nToken: %s",token);
    }
printf ("\nsize was: %i", cmdlist_size);
    j = 0;
    k = 0;

    for (i = 0; i < cmdlist_size; i++) {
        unsigned int type = commandType(cmd_list[i]);

					printf("\nThe command type is: %i", type);
        switch (type) {
        case 0: // Normal command, just add to argv list of process object
            proclist[j].argv[k++] = cmd_list[i];
            break;

        case 1: // pipe
            proclist[j].flags = proclist[j].flags | redirect_pipe_output;
            proclist[j+1].flags = proclist[j+1].flags | redirect_pipe_input;
            proclist_size++;
            j++;
            k = 0;
            break;

        case 2:
            proclist[j].flags = proclist[j].flags | redirect_output_to_file;
            proclist[j].filename = cmd_list[++i];
            j++;
            break;

        case 3:
						proclist[j].flags = proclist[j].flags | redirect_output_to_file_append;
						proclist[j].filename = cmd_list[++i];
            break;

        case 4:
						proclist[j].flags = proclist[j].flags | redirect_error_to_file;
						proclist[j].filename = cmd_list[++i];
            break;

        case 5:
						proclist[j].flags = proclist[j].flags | redirect_error_to_file_append;
						proclist[j].filename = cmd_list[++i];

            break;

        case 6:
						proclist[j].flags = proclist[j].flags | redirect_file_to_input;
						proclist[j].filename = cmd_list[++i];
            break;

        case 7:
						// run in background
            break;

        case 8:
        case 9:
        case 10:
						proclist[j].flags = proclist[j].flags | is_builtin;
						proclist[j].argv[0]  = cmd_list[++i];
            break;
        };

    }
        return ++proclist_size;
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

int mainloop(char** envp) {
    struct Proc process_list[50];
    size_t i,j;

    for (i = 0; i < 50; i++) {
        struct Proc newproc;
        process_list[i] = newproc;
        process_list[i].flags = 0;
        process_list[i].filename = "\0";

        for (j = 0; j < 50; j++) {
            process_list[i].argv[j] = 0;
        }
    }

    size_t size = parseInput(process_list);
    pid_t pid;
    int pipefd[2];
    pipe(pipefd);

    for (i = 0; i < size; i++) {
			run(process_list[i], &pid, pipefd);
    }

    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    while (!mainloop(envp)) continue;
}
