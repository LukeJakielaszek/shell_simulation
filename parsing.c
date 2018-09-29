#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#define BUFSIZE 50

void parsePipe(char * pipeSeg);
void parseLine(char * line);
char * readLine();

int main(){
  char * input = readLine();

  parseLine(input);

  
  
  return 0;
}

char * readLine(){
  char * buffer = (char*)malloc(sizeof(char)*BUFSIZE);

  // checks for malloc success
  if(buffer == NULL){
    printf("ERROR: Command Line Parsing buffer failed to malloc\n");
    exit(-1);
  }

  // checks for read success
  if(read(STDIN_FILENO, buffer, BUFSIZE) < 0){
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

  // tokenize the rest of the line
  while(token != NULL){

    // parse pipe command by whitespace
    parsePipe(token);

    // get next token
    token = strtok(NULL, delim);
  }
}

void parsePipe(char * pipeSeg){
  
}
