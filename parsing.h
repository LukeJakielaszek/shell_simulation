#ifndef PARSING_H
#define PARSING_H

#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include "llist.h"

#define BUFFSIZE 50

void parsePipe(char * pipeSeg, llist1 *commandSet);
llist2 *parseLine(char * line);
char * readLine();
int isTab(char c);
int pipingIsInvalid(char * inputLine);

#endif /** PARSING_H */
