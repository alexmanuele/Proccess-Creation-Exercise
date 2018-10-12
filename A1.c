#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80

struct pastArg{
  int pid;
  char args[MAX_LINE + 1];
};
  
void addToHistory(struct pastArg history[10], char args[MAX_LINE +1], int pid);
void printHistory(struct pastArg history[10]);
  
int main(void)
{
  pid_t pid;
  char *args[MAX_LINE/2 + 1];
  struct pastArg history[10] = {{-1, "",},{-1, "",},{-1, "",},{-1, "",},{-1, "",},{-1, "",},{-1, "",},{-1, "",},{-1, "",},{-1, "",}};
  int should_run = 1;
  char rawInput[MAX_LINE + 1]; //for the input
  char* token;
  char command[MAX_LINE + 1];
  int i = 0;
  int j =0;
  int should_wait = 0;
  while(should_run){
    i = 0;
    should_wait = 0;
    printf("CSCI3120>");
    fflush(stdout);
    
    //Read the input
    fgets(rawInput, MAX_LINE+1, stdin);
  command: strcpy(command, rawInput);
    //Tokenize the string and store in args
    token = strtok(rawInput, " ");
    while(token){
      args[i] = token;
      i++;
      token = strtok(NULL, " ");
    }
    //execvp() requires a null terminated list of args
    args[i] = NULL;
    //chekc if command was exit
    if(strcmp(args[0], "exit\n") == 0 || strcmp(args[0], "exit") == 0){
	should_run = 0;
      }
     
    //check if last argument was &
    if(strcmp(args[i-1], "&") != 0){
      should_wait = 1;
      }
    //check if command was history
     if(strcmp(args[0], "history\n") == 0 || strcmp(args[0], "history") == 0){
         printHistory(history);
      }
     //check for !x commands
     if(strcmp(args[0], "!10\n") == 0 || strcmp(args[0], "!10") == 0){
        if(history[9].pid == -1){
	     printf("There are less than 10 commands in history.\n");
	   }
	   else{
	     strcpy(rawInput, history[9].args);
	     goto command;
	   }
     }
     else  if(strlen(args[0])==3 && args[0][0] == '!'){
       if(history[0].pid == -1){
	 printf("No commands in history.\n");
       }
       else{
	 switch (args[0][1]){
	     case '!':
	       j = 0;
	       break;
	     case '1':
	       j = 1;
	       break;
	     case '2':
	       j = 1;
	       break;
	     case '3':
	       j = 2;
	       break;
	     case '4':
	       j = 3;
	       break;
	     case '5':
	       j = 4;
	       break;
	     case '6':
	       j = 5;
	       break;
	     case '7':
	       j = 6;
	       break;
	     case '8':
	       j = 7;
	       break;
	     case '9':
	       j = 8;
	       break;
	     default:
		 j = -1;
		 printf("Invalid input\n");
	      
	   }
	 if( j >= 0){
	   if(history[j].pid == -1){
	     printf("There are less than %d commands in history.\n", j+1);
	   }
	   else{
	     strcpy(rawInput, history[j].args);
	     goto command;
	   }
	 }
       }
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
	 exit(0);
     }
     else{
       //parent
       if(should_wait){
	 wait(NULL);
       }
       addToHistory(history, command, pid); //pid is the pid of child
    }
  }
  return 0;
}
void addToHistory(struct pastArg history[10], char args[MAX_LINE +1], int pid){
  int i = 9;
  while (i > 0){
    history[i] = history[i-1];
    i--;
  }
  struct pastArg argument;
  argument.pid = pid;	   
  strcpy(argument.args, args);
  history[0] = argument;
}

void printHistory(struct pastArg history[10]){
  char *prompt = {"ID\tPID\tCommand"};
  char *out;
  int i = 0;
  if(history[0].pid == -1){
    printf("There are no commands in history.\n");
    return;
  }
  printf("%s\n", prompt);
  while(i < 10){
    if(history[i].pid != -1){
      printf("%d\t%d\t%s",i+1, history[i].pid, history[i].args);
      i++;
    }
    else{
      break;
    }
  }
  printf("\n");
  return;
}
