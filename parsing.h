#ifndef PARSING_H
#define PARSING_H

#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include "llist.h"

#define BUFFSIZE 50

void parsePipe(char * pipeSeg, llist1 *commandSet, int * syntaxFlag,
	       int pipSize);
llist2 *parseLine(char * inputLine, int * syntaxFlag);
char * readLine(int * shouldWait);
int isTab(char c);
int pipingIsInvalid(char * inputLine);
int getSize(char * buffer);
void checkCommand(llist1 * commandSet, int * syntaxFlag, int pipSize);

#endif /** PARSING_H */
