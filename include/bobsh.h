/*
 * bobshell, a crappy shell that does a thing.
 *
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/boberr.h"
#define buffer_size 1024 //buffer size for read/write functions.
#define max_arg_count 50 //max amount of arguments that can be passed.
/*
int run();
int makeArgvList(char** argv_list[], const char* command_list[]);
int cmdPrompt(char* command_list[]);
int runBuiltins(const char* command_list[]);


*/

int mainLoop(char** env);
int parseInput(char** argv_list[]);
int run(const char** argv_list[], char** env);
// builtins
int runBuiltins(const char* command);
int builtin_pipe();
int builtin_inputRedirect(); // <
int builtin_outputRedirect();
int builtin_errorRedirect();
int builtin_outputAppend();
int builtin_output();

