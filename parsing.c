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

/*
int main(){
  char * input = readLine();

  if(pipingIsInvalid(input)){
    printf("Invalid Command: Adjacent pipes detected.\n");
    return -1;
  }
  
  llist2 * pipeSections = parseLine(input);

  printList2(pipeSections);

  return 0;
}
*/

// reads a line from std_in
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

  // removes \n from end of string
  buffer[strlen(buffer)-1] = '\0';

  // returns read line
  return buffer;
}

// checks if user input has adjacent pipes "||", does not check for empty
// pipes "| |". Also checks if first char is a pipe, if so, returns 1
int pipingIsInvalid(char * inputLine){
  int lineSize = strlen(inputLine);
  int i = 0;

  // set to 1 if previous char was a pipe, 0 otherwise
  int wasPipe = 0;

  // loops through input searching for adjacent pipes.
  for(i = 0; i < lineSize; i++){
    if(inputLine[i] == '|'){
      // if the current char is a pipe
      if(wasPipe){
	// and the previous char was a pipe, return true
	return 1;
      }else{
	// and the previous char was not a pipe, set wasPipe to true
	wasPipe = 1;
      }
    }else{
      // if the current char is not a pipe, set wasPipe to false
      wasPipe = 0;
    }
  }

  // checks if first char is a pipe
  if(inputLine[0] == '|'){
    return 1;
  }

  // returns false if adjacent pipes do not exist
  return 0;
}

// parse an entire line of user input into a 2D list of pipe sections
llist2 *parseLine(char * inputLine){
  // sets delim to pipe
  const char delim[2] = "|";

  char * token;
  
  // grab first token
  token = strtok(inputLine, delim);

  llist1 * pipList = makeList1();

  int size = 0;
  // tokenize the rest of the line
  while(token != NULL){
    // appends each pipe-seperated string to a temp list
    append1(pipList, token);

    size++;
    
    // get next token
    token = strtok(NULL, delim);
  }

  // 2D list to hold parse input
  llist2 * pipeSections = makeList2();
  
  // parses each pipe section into 1D list, splitting by whitespace and
  // appends to 2D list
  int i = 0;
  for(i = 0; i < size; i++){
    // 1D list to hold whitespaced parsed commands
    llist1 * commandSet = makeList1();

    // parse each pipe section
    parsePipe(get1(pipList, i), commandSet);

    // appends pipe section to list of pipeSections
    append2(pipeSections, commandSet);
  }

  return pipeSections;
}

// parses pipe section by whitespace, appending them to a 1D commandSet list
void parsePipe(char * pipeSeg, llist1 *commandSet){
  int i = 0;
  
  // gets length of pipe string section
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
    // append each command to a 1D list
    append1(commandSet, token);
    
    // get next token
    token = strtok(NULL, delim);
  }
}

// returns true if char is a tab
int isTab(char c){
  return c == '\t';
}
