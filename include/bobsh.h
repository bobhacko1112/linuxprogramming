/*
 * bobshell, a crappy shell that does a thing.
 *
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "../include/boberr.h"
#define redirect_pipe_output 1
#define redirect_pipe_input 2
#define redirect_output_to_file 4
#define redirect_error_to_file 8
#define redirect_output_to_file_append 16
#define redirect_error_to_file_append 32
#define run_background 64
#define redirect_error_to_output 128
#define redirect_file_to_input 256
#define is_builtin 512
#define buffer_size 1024 //buffer size for read/write functions.
#define max_arg_count 50 //max amount of arguments that can be passed.
