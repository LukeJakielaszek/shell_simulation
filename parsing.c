#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include "llist.h"

#define BUFFSIZE 50

void parsePipe(char * pipeSeg, llist1 *commandSet, int * syntaxFlag,
	       int pipSize, int commandLen);
llist2 *parseLine(char * inputLine, int * syntaxFlag);
char * readLine();
int isTab(char c);
int pipingIsInvalid(char * inputLine);
int getSize(char * buffer);
void checkCommand(llist1 * commandSet, int * syntaxFlag, int pipSize,
		  int commandLen);

// reads a line from std_in
char * readLine(){
  size_t bufferSize = BUFFSIZE;
  char * buffer = (char*)malloc(sizeof(char)*bufferSize);

  // checks for malloc success
  if(buffer == NULL){
    printf("ERROR: Command Line Parsing buffer failed to malloc\n");
    exit(-1);
  }

  if(getline(&buffer, &bufferSize, stdin) < 0){
    printf("ERROR: Hit end of file without quitting.\n");
    exit(EXIT_FAILURE);
  }

  int lenRead = getSize(buffer);
  
  buffer[lenRead] = '\0';
  
  // returns read line
  return buffer;
}

int getSize(char * buffer){
  int i = 0;
  while(buffer[i] != '\n'){
    i++;
  }

  return i;
}

// checks if user input has adjacent pipes "||", does not check for empty
// pipes "| |". Also checks if first char or last char are pipes,
// if so, returns 1
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

  // checks if last char is a pipe
  if(inputLine[strlen(inputLine)-1] == '|'){
    return 1;
  }

  // returns false if adjacent pipes do not exist
  return 0;
}

// parse an entire line of user input into a 2D list of pipe sections
llist2 *parseLine(char * inputLine, int * syntaxFlag){
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

    // if a syntax error is detected, return to game loop.
    if(*syntaxFlag){
      return pipeSections;
    }
    
    // parse each pipe section
    parsePipe(get1(pipList, i), commandSet, syntaxFlag, pipeSections->size,
	      size);

    // appends pipe section to list of pipeSections
    append2(pipeSections, commandSet);
  }

  return pipeSections;
}

// parses pipe section by whitespace, appending them to a 1D commandSet list
void parsePipe(char * pipeSeg, llist1 *commandSet, int * syntaxFlag,
	       int pipIndex, int commandLen){
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

  checkCommand(commandSet, syntaxFlag, pipIndex, commandLen);
}

// checks for syntax errors in user input, sets syntaxflag to 1 if found
void checkCommand(llist1 * commandSet, int * syntaxFlag, int pipIndex,
		  int commandLen){
  // checks if piping is empty
  if(isEmpty1(commandSet)){
    printf("ERROR: Input includes empty piping.\n");
    *syntaxFlag = 1;
  }

  // set to 1 when finding corresponding redirect symbol
  int redirectInFlag = 0;
  int redirectOutFlag = 0;
 
  // checks redirect ordering and ammount
  int i = 0;
  for(i = 0; i < commandSet->size; i++){
    // gets word in pipesection
    char * word = get1(commandSet, i);

    char * nextWord;
    // gets next word if it exists
    if(i+1 < commandSet->size){
      nextWord = get1(commandSet, i+1);
    }else{
      nextWord = NULL;
    }

    // checks for redirection
    if(strcmp(word, "<") == 0){
      // looks for in redirection
      redirectInFlag++;

      // if input redirect occurs after a pipe
      if(pipIndex > 0){
	printf("ERROR: Input redirect detected in sucessive piping.\n");
	*syntaxFlag = 1;
      }
      
      // if redirect out occurs before in
      if(redirectOutFlag > 0){
	printf("ERROR: Output redirect detected before input redirect.\n");
	*syntaxFlag = 1;
      }

      if(nextWord == NULL || strcmp(nextWord, ">") == 0){
	printf("ERROR: No file detected to redirect input to.\n");
	*syntaxFlag = 1;
      }
      
    }else if(strcmp(word, ">") == 0 || strcmp(word, ">>") == 0){
      // looks for output redirection
      redirectOutFlag++;

      // checks if an output redirect occurs before a pipe
      if(pipIndex != commandLen-1){
	printf("ERROR: Output redirect detected before final command.\n");
	*syntaxFlag = 1;
      }

      if(nextWord == NULL){
	printf("ERROR: No file detected to redirect output to.\n");
	*syntaxFlag = 1;
      }
    }

    // checks if too many redirects occured
    if(redirectInFlag > 1 || redirectOutFlag > 1){
      printf("ERROR: Too many redirects detected within input.\n");
      *syntaxFlag = 1;
    }

    // stop processing if syntax error is detected.
    if(*syntaxFlag){
      return;
    }
  }
}

// returns true if char is a tab
int isTab(char c){
  return c == '\t';
}

