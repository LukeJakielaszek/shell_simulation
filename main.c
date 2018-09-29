#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "llist.h"
#include "parsing.h"

int main(int argc, char * argv, char **environ){
  // change input stream to file if requested
  if(argc == 2){
    
  }

  // grabs PWD environment variable
  char *PWD = "ShellPath> ";
  
  // simulation loop
  while(1){

    // writes to std_out, system call flushes output buffer
    write(STDOUT_FILENO, PWD, strlen(PWD));

    // reads user input
    char *inputLine = readLine();

    // processes input
    if(strlen(inputLine) == 0){
      // continues if empty string is passed
      continue;
    }else if(strcmp(inputLine, "exit") == 0){
      // if exit is entered, ends program
      return 0;
    }else if(pipingIsInvalid(inputLine)){
      // if paping is invalid, continues
      printf("ERROR: Invalid piping detected.\n");
      continue;
    }else{
      // parses user input by piping and whitespace
      llist2 * pipeSections = parseLine(inputLine);
    
      printList2(pipeSections);
    }
  }
}
