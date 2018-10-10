// native libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// self made libraries
#include "llist.h"
#include "parsing.h"
#include "utilities.h"

// piping constants
#define READ 0
#define WRITE 1
#define BUFSIZE 50

// holds terminal file descriptor
int savedStdOut;

// holds a global env
char ** env;

// holds line for echo
char line[100];

// helpfile path
char *helpFile = "help.md";

// prototypes
void firstCommand(llist2 * userInputList, int shouldWait);
void pipingRec(llist2 * userInputList);
void processCommand(llist1 * commandList);
int isBuiltIn(char * command);

int main(int argc, char ** argv, char **environ){
  env = environ;
  
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

  // simulation loop
  while(1){
    // defaults program to wait
    int shouldWait = 1;
    
    char *PWD = getenv("PWD");

    // writes PWD to terminal
    write(STDOUT_FILENO, PWD, strlen(PWD));
    write(STDOUT_FILENO, "> ", 2);

    // reads user input
    char *inputLine = readLine(&shouldWait);
    
    // store line for echo
    strcpy(line, inputLine);

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
	    continue;
	  }else if(temp->size == 1){
	    printf("\n");
	    continue;
	  }else{
	    printf("ERROR: Invalid arguments for cd.\n");
	    continue;
	  }
	}
      }

      // branches from main process and performs piping/redirection
      firstCommand(pipeSections, shouldWait);
    }
  }
}

// seperates process from main program
void firstCommand(llist2 * userInputList, int shouldWait){
  // forks process
  pid_t pid = fork();
  int status = 0;

  if(pid == -1){
    // checks for failed fork
    printf("ERROR: First fork failed to fork.\n");
    exit(EXIT_FAILURE);
  }else if(pid == 0){
    // child process executes commands
    if(userInputList->size > 1){
      // recursively set up pipes if at least 1 pipe exists
      pipingRec(userInputList);
    }else{
      // store final commandset
      llist1 *temp = pop2(userInputList);

      // free leftover 2D list
      free(userInputList);
      
      // directly process a command if no piping exists
      processCommand(temp);
    }
    
    exit(EXIT_SUCCESS);
  }else{
    // parent process either waits for child or immediately begins next
    // round of user input
    if(shouldWait){
      pid_t temp = pid;

      // waits for the specific child to finish
      do{
	// trys to wait to child to finish
	pid = waitpid(temp, &status, WNOHANG);

	// checks wait fail
	if(pid == -1){
	  printf("ERROR: Failed to wait.\n");
	  exit(EXIT_FAILURE);
	}

	// sleep for half a second to not waste cpu time
	sleep(0.5);
      }while(pid != temp);
    }
  }
}

// recursivley sets up piping and forking
void pipingRec(llist2 * userInputList){
  int fd[2];
  pid_t pid;
  int status = 0;

  // sets up pipe
  if(pipe(fd) == -1){
    printf("ERROR: Failed to create pipe.\n");
    exit(EXIT_FAILURE);
  }

  llist1 * tempList = pop2(userInputList);
  
  // forks process
  pid = fork();
  if(pid == -1){
    // if fork error
    printf("ERROR: Failed to fork process.\n");
    exit(EXIT_FAILURE);
  }else if(pid == 0){
    // child process
    // closes read portion of pipe
    if(close(fd[READ]) == -1){
      printf("ERROR: Failed to close read portion of pipe.\n");
      exit(EXIT_FAILURE);
    }

    // redirects output of child to pipe
    if(dup2(fd[WRITE], STDOUT_FILENO) < 0){
      printf("ERROR: Failed to swap output stream to pipe.\n");
      exit(EXIT_FAILURE);
    }

    // calls command to process 1D list
    processCommand(tempList);

    // kills process if still exists
    exit(EXIT_SUCCESS);
  }else{
    // parent process
    // closes write portion of pipe
    if(close(fd[WRITE]) == -1){
      printf("ERROR: Failed to close write portion of pipe.\n");
      exit(EXIT_FAILURE);
    }

    // redirects input of parent to pipe
    if(dup2(fd[READ], STDIN_FILENO) < 0){
      printf("ERROR: Failed to swap input stream to pipe.\n");
      exit(EXIT_FAILURE);
    }

    pid = wait(&status);

    if(pid == -1){
      printf("ERROR: Failed to wait.\n");
      exit(EXIT_FAILURE);
    }
    
    // recursively calls piping until list is empty 
    if(userInputList->size > 1){
      // recursively call piping function, passing in the shortened list
      pipingRec(userInputList);
    }else{
      // restore terminal output and process final command
      // redirects output of parent to terminal
      if(dup2(savedStdOut, STDOUT_FILENO) < 0){
	printf("ERROR: Failed to restore output stream to terminal.\n");
	exit(EXIT_FAILURE);
      }

      // get final command
      llist1 *temp = pop2(userInputList);

      // free leftover 2D list
      free(userInputList);
      
      // process final command
      processCommand(temp);

      // frees final section of list
      free(userInputList);
      
      exit(EXIT_SUCCESS);
    }
  }
}

// processes a single pipe's commands
void processCommand(llist1 * commandList){
  int i = 0;

  //store size
  int end = commandList->size;
  
  // default trackers to false
  int inIndex = 0;
  int outIndex = 0;
  int outCatIndex = 0;
  for(i = 0; i < end; i++){
    // stores indices for redirects
    if(strcmp(get1(commandList, i), ">") == 0){
      // if an ouput redirect is detected store the index
      outIndex = i;
    }else if(strcmp(get1(commandList, i), ">>") == 0){
      // if an ouput redirect is detected store the index
      outCatIndex = i;
    }else if(strcmp(get1(commandList, i), "<") == 0){
      // if an ouput redirect is detected store the index
      inIndex = i;
    } 
  }

  // redirects output to the specified file stream
  if(outIndex){
    redirectOutput(get1(commandList, outIndex+1));
  }else if(outCatIndex){
    redirectOutputCat(get1(commandList, outCatIndex+1));
  }

  // redirects input stream to the specified file
  if(inIndex){
    redirectInput(get1(commandList, inIndex+1));
  }

  
  // finds where argv for executables ends
  if(end-1 > outIndex && outIndex > 0){
    end = outIndex;
  }else if (end-1 > outCatIndex && outCatIndex > 0){
    end = outCatIndex;
  }else if(end-1 > inIndex && inIndex > 0){
    end = inIndex;
  }

  // mallocs argv
  char ** argv = (char **)malloc(sizeof(char*)*end+1);

  // checks for failure
  if(argv == NULL){
    printf("ERROR: Failed to malloc argv.\n");
    exit(EXIT_FAILURE);
  }

  // gets argv
  for(i = 0; i < end; i++){
    argv[i] = get1(commandList, i);
  }

  // set final index of argv to NULL
  argv[end] = NULL;

  // runs command
  if(strcmp(argv[0], "dir") == 0){
    // command is builtin dir function

    // checks if correct num params are entered
    if(end > 2){
      printf("ERROR: Dir takes one or zero parameters.\n");
      exit(EXIT_FAILURE);
    }else if(end == 1){
      // defaults to current directory
      dir(".");
    }else{
      // dirs desired directory
      dir(argv[1]);
    }
  }else if(strcmp(argv[0], "environ") == 0){
    // command is builtin environ function

    // prints environ
    if(end > 1){
      // checks for invalid params
      printf("ERROR: Env does not take parameters.\n");
      exit(EXIT_FAILURE);
    }else{
      // prints environ
      printEnv(env);
    }
  }else  if(strcmp(argv[0], "echo") == 0){
    // command is builtin echo function
    if(end > 1){
      // prints line passed in by user
      echoLine(line);
    }else{
      // print whats in std in
      echoInput();
    }
  }else  if(strcmp(argv[0], "help") == 0){
    // command is builtin help function
    help(helpFile);

    printf("\n");
  }else{
    // temp variables
    node * nodeIter = commandList->head;
    node * next;
    
    // frees 1D linked list
    while(nodeIter != NULL){
      next = nodeIter->next;
      free(nodeIter);
      nodeIter = next;
    }

    // the program is an executable
    execvp(argv[0], argv);

    // prints error if made it here
    printf("ERROR: Failed to exec %s\n", argv[0]);
  }
  
  
  // temp variables
  node * nodeIter = commandList->head;
  node * next;
  
  // frees 1D linked list
  while(nodeIter != NULL){
    next = nodeIter->next;
    free(nodeIter);
    nodeIter = next;
  }
}

// checks if a string is a pipable builtin function
int isBuiltIn(char * command){
  return (strcmp(command, "dir") == 0 || strcmp(command, "environ") == 0
	  || strcmp(command, "echo") == 0 || strcmp(command, "help") == 0);
}
