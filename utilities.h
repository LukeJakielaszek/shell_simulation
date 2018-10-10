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
#include <sys/stat.h>

#define BUFSIZE 50

// prototypes
void redirectInput(char * newInput);
void clearScreen(int n);
void changeDirectory(char * PWD, const char * newDir);
void dir(char * path);
void redirectOutput(char * newInput);
void redirectOutputCat(char * newOutput);
void printEnv(char ** environment);
void echoLine(char * line);
void echoInput();
void help(char * helpFile);

#endif /** UTILITIES_H */
