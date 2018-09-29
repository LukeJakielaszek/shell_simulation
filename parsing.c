#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include "llist.h"

#define BUFFSIZE 50

void parsePipe(char * pipeSeg);
void parseLine(char * line);
char * readLine();
int isTab(char c);

int main(){
  char * input = readLine();

  parseLine(input);

  return 0;
}

char * readLine(){
  char * buffer = (char*)malloc(sizeof(char)*BUFFSIZE);

  // checks for malloc success
  if(buffer == NULL){
    printf("ERROR: Command Line Parsing buffer failed to malloc\n");
    exit(-1);
  }

  // checks for read success
  if(read(STDIN_FILENO, buffer, BUFFSIZE) < 0){
    printf("ERROR: Failed to read line from std_in\n");
    exit(-1);
  }

  // returns read line
  return buffer;
}

// parse an entire line of user input by pipes
void parseLine(char * inputLine){
  // gets length of input
  int lineLen = strlen(inputLine);

  // checks if nothing was entered
  if(lineLen == 1){
    printf("line length : 0\n");
    return;
  }
  
  // removes \n from end of string
  inputLine[lineLen-1] = '\0';

  // sets delim to pipe
  const char delim[2] = "|";

  char * token;
  
  // grab first token
  token = strtok(inputLine, delim);

  llist1 * pipList = makeList1();
  
  // tokenize the rest of the line
  while(token != NULL){
    // appends each pipe-seperated string to a temp list
    append1(pipList, token);
    
    // get next token
    token = strtok(NULL, delim);
  }

  int i = 0;
  for(i = 0; i < pipList->size; i++){
    printf("Segment %d\n", i);
    parsePipe(get1(pipList, i));
  }
}

void parsePipe(char * pipeSeg){
  int i = 0;
  int pipLen = strlen(pipeSeg);

  // converts all tabs to spaces
  for(i = 0; i<pipLen; i++){
    if(isTab(pipeSeg[i])){
      pipeSeg[i] = ' ';
    }
  }

  
  // sets delim to space
  const char delim[2] = " ";

  char * token;


  // grab first token
  token = strtok(pipeSeg, delim);

  
  // tokenize the rest of the line
  while(token != NULL){
    printf("\t[%s]\n", token);
    // get next token
    token = strtok(NULL, delim);
  }
}

// returns true if char is a tab
int isTab(char c){
  return c == '\t';
}
