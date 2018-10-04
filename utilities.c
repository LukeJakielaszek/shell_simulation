// native libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// prototypes
void redirectInput(char * newInput);
void clearScreen(int n);

// changes working directory
void changeDirectory(){
  
}

// clears screen by printing n newlines
void clearScreen(int n){
  int i = 0;
  for(i = 0; i < n; i++){
    printf("\n");
  }
}

// changes the input stream of current process to specified file descriptor
void redirectInput(char * newInput){
  int fd;
  
  // opens new input file, checks for success
  if((fd = open(newInput, O_RDONLY)) < 0){
    printf("ERROR: Failed to open %s.\n", newInput);
    exit(EXIT_FAILURE);
  }

  // change input file stream, check for success
  if(dup2(fd, STDIN_FILENO) < 0){
    printf("ERROR: Failed to swap input stream to %s.\n", newInput);
    exit(EXIT_FAILURE);
  }
}
