#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80

int main(void)
{
  pid_t pid;
  char *args[MAX_LINE/2 + 1]; //an array of strings apparently
  int should_run = 1;
  char rawInput[MAX_LINE + 1]; //for the input
  char* token;
  int i = 0;
  int should_wait = 0;
  while(should_run){
    i = 0;
    should_wait = 0;
    printf("CSCI3120>");
    fflush(stdout);
    
    //Read the input
    fgets(rawInput, MAX_LINE+1, stdin);
    //Tokenize the string and store in args
    token = strtok(rawInput, " ");
    while(token){
      args[i] = token;
      i++;
      token = strtok(NULL, " ");
    }
    //chekc if command was exit
    if(strcmp(args[0], "exit\n") == 0 || strcmp(args[0], "exit") == 0){
	should_run = 0;
      }
    //check if last argument was &
    if(strcmp(args[i-1], "&") != 0){
      should_wait = 1;
      }
    
    //fork child process
    pid = fork();
    if(pid == -1){
      //error
      exit(EXIT_FAILURE);
    }
    else if (pid == 0){
      //child
	execvp(args[0], args);
	printf("Child doing something\n");
    }
    else{
      //parent
      if(should_wait){
	wait(NULL);
       }
      printf("Parent doing something\n");
    }
  }
  return 0;
}
