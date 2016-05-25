#ifndef _PIPE_H
#define _PIPE_H
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <fcntl.h>

#include "parsing.h"
#include "cat.h"
#include "touch.h"

#define BUFF 1024 
#define PATH_SIZE 1024
#define MAX_ARGS_SIZE  512

void execPipe(char *buff, int npipes, char *history_file_path, char *dir);


#endif
