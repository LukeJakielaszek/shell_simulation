// native libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 50

// prototypes
void redirectInput(char * newInput);
void clearScreen(int n);
void changeDirectory(char * PWD, const char * newDir);
void dir(char * path);
void redirectOutput(char * newInput);

void dir(char * path){
  // opens desired directory
  DIR * dir = opendir(path);

  // file descriptor of each file in directory
  struct dirent *fd;

  // checks for failed opendir
  if(dir == NULL){
    printf("ERROR: Directory does not exist");
    return;
  }

  int i = 0;
  int size = BUFSIZE;

  // reads directory, append to retchar
  while((fd = readdir(dir)) != NULL){
    // copy filename into retchar
    printf("[%s]\n", fd->d_name);
  }

  closedir(dir);
}

// changes working directory
void changeDirectory(char * PWD, const char * newDir){
  // gets absolute path
  char * absPath = realpath(newDir, NULL);

  // checks for success
  if(absPath == NULL){
    printf("ERROR: Unable to find directory.\n");
    return;
  }

  // changes directory
  if(chdir(absPath) != 0){
    printf("ERROR: Unable to find directory.\n");
    return;
  }

  // copies new path into environment
  strcpy(PWD, absPath);

  // frees the absolute path
  free(absPath);
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

// changes the output stream of current process to specified file descriptor
void redirectOutput(char * newInput){
  int fd;
  
  // opens new input file, checks for success
  if((fd = open(newInput, O_WRONLY|O_CREAT|S_IRWXU|S_IRWXG|S_IRWXO)) < 0){
    printf("ERROR: Failed to open %s.\n", newInput);
    exit(EXIT_FAILURE);
  }

  // change input file stream, check for success
  if(dup2(fd, STDOUT_FILENO) < 0){
    printf("ERROR: Failed to swap output stream to %s.\n", newInput);
    exit(EXIT_FAILURE);
  }
}
