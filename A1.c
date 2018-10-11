#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 80

int main(void)
{
  char *args[MAX_LINE/2 + 1]; //an array of strings apparently
  int should_run = 1;
  char rawInput[MAX_LINE + 1]; //for the input
  char* token;
  int i = 0;
  while(should_run){
    i = 0;
    printf("CSCI3120>");
    fflush(stdout);
    /**
     *Fork
     *chuld invoke execvp()
     *parent will not wait if & included
     **/
    //Read the input
    fgets(rawInput, MAX_LINE+1, stdin);
    //Tokenize the string and store in args
    token = strtok(rawInput, " ");
    while(token){
      args[i] = token;
      i++;
      token = strtok(NULL, " ");
    }
    printf("%s, %s", args[0], args[1]);
    should_run = 0;
  }
  printf("%s", rawInput);
  return 0;
}
