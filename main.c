#include <stdio.h>
#include <unistd.h>

extern char *environ[];

int main(void){

  environ[0] = "home";
  
  printf("%s>", environ["PATH"]);
  
}
