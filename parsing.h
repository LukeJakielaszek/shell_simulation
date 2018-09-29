#ifndef PARSING_H
#define PARSING_H

#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define BUFSIZE 50

void parsePipe(char * pipeSeg);
void parseLine(char * line);
char * readLine();

#endif /** PARSING_H */
