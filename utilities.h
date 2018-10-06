#ifndef UTILITIES_H
#define UTILITIES_H

// native libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

// prototypes
void redirectInput(char * newInput);
void clearScreen(int n);
void changeDirectory(char * PWD, const char * newDir);
void dir(char * path);

#endif /** UTILITIES_H */
