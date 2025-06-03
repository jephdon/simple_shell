#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

extern char **environ; /* Global environment viriable */

void print_t(char *str);
char *find_command(char *command);
void print_env(void);

#endif /* MAIN_H */
