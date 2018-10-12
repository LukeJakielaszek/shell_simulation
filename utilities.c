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
void redirectOutputCat(char * newOutput);
void printEnv(char ** environment);
void echoLine(char * line);
void echoInput();
void help(char * helpFile);

void help(char * helpFile){
  redirectInput(helpFile);
  
  size_t bufferSize = BUFSIZE;
  char * buffer = (char*)malloc(sizeof(char)*bufferSize);

  // checks for malloc success
  if(buffer == NULL){
    printf("ERROR: Command Line Parsing buffer failed to malloc\n");
    exit(-1);
  }

  
  
  while(getline(&buffer, &bufferSize, stdin) > 0){
    printf("%s", buffer);
  }
}

void echoInput(){
  size_t bufferSize = BUFSIZE;
  char * buffer = (char*)malloc(sizeof(char)*bufferSize);

  // checks for malloc success
  if(buffer == NULL){
    printf("ERROR: Command Line Parsing buffer failed to malloc\n");
    exit(-1);
  }

  while(getline(&buffer, &bufferSize, stdin) > 0){
    printf("%s", buffer);
  }
}

// echos a line from terminal
void echoLine(char * line){
  // prints command line line
  int i = 4;
  int len = strlen(line);

  // print everything after echo
  for(i = 5; i < len; i++){
    printf("%c", line[i]);
  }

  printf("\n");
}

// prints environmnet to std out
void printEnv(char ** environment){
  int i = 0;

  // loops through environ
  while(environment[i] != NULL){
    printf("%s\n", environment[i]);
    i++;
  }
}

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
    printf("%s\n", fd->d_name);
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
void redirectOutput(char * newOutput){
  int fd;

  // opens new input file, checks for success
  if((fd = open(newOutput, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
    printf("ERROR: Failed to open %s.\n", newOutput);
    exit(EXIT_FAILURE);
  }

  // change input file stream, check for success
  if(dup2(fd, STDOUT_FILENO) < 0){
    printf("ERROR: Failed to swap output stream to %s.\n", newOutput);
    exit(EXIT_FAILURE);
  }
}

// changes the output stream of current process to specified file descriptor
void redirectOutputCat(char * newOutput){
  int fd;
  
  // opens new input file, checks for success
  if((fd = open(newOutput, O_WRONLY|O_APPEND|O_CREAT, 0644))
     < 0){
    printf("ERROR: Failed to open %s.\n", newOutput);
    exit(EXIT_FAILURE);
  }

  // change input file stream, check for success
  if(dup2(fd, STDOUT_FILENO) < 0){
    printf("ERROR: Failed to swap output stream to %s.\n", newOutput);
    exit(EXIT_FAILURE);
  }
}
