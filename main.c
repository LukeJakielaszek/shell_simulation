// native libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// self made libraries
#include "llist.h"
#include "parsing.h"
#include "utilities.h"

int savedStdOut;

int main(int argc, char ** argv, char **environ){
  // change input stream to file if requested
  if(argc == 2){
    // store new file descriptor
    redirectInput(argv[1]);
  }else if (argc > 2){
    // exits on invalid program initialization
    printf("ERROR: Too many arguments passed into main.\n");
    return EXIT_FAILURE;
  }

  // saves std out, checks for success
  if((savedStdOut=dup(STDOUT_FILENO)) == -1){
    printf("ERROR: Failed to save std_out for main process.\n");
    return EXIT_FAILURE;
  }

  // grabs PWD environment variable
  char *PWD = getenv("PWD");
  
  // simulation loop
  while(1){

    // writes PWD to terminal
    write(STDOUT_FILENO, PWD, strlen(PWD));
    write(STDOUT_FILENO, "> ", 2);

    // reads user input
    char *inputLine = readLine();
    
    // processes input
    if(strlen(inputLine) == 0){
      // continues if empty string is passed
      printf("\n");
      continue;
    }else if(strcmp(inputLine, "quit") == 0){
      // if exit is entered, ends program
      return EXIT_SUCCESS;
    }else if(strcmp(inputLine, "clr") == 0){
      // clears screen by printing newLines
      clearScreen(41);
    }else if(pipingIsInvalid(inputLine)){
      // if piping is invalid, continues
      printf("ERROR: Invalid piping detected.\n");
      continue;
    }else{
      int syntaxFlag = 0;
      
      // parses user input by piping and whitespace
      llist2 * pipeSections = parseLine(inputLine, &syntaxFlag);
      
      // if syntax error is detected, continues next loop iteration
      if(syntaxFlag){
	continue;
      }

      // checks for cd
      if(pipeSections->size == 1 ){
	llist1 * temp = peek(pipeSections);
	if(strcmp(get1(temp, 0), "cd") == 0){
	  if(temp->size == 2){
	    changeDirectory(PWD, get1(temp, 1));
	  }else{
	    printf("ERROR: Invalid arguments for cd.\n");
	    continue;
	  }
	}
      }
      
      printList2(pipeSections);
    }
  }
}
