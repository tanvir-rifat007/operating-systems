#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define MAX_CMD 1024
#define PROMPT "shell> "
#define MAX_ARGV 256
#define SEP " \t\n"

volatile pid_t child_pid = 0;

void sig_handler(int sig){

   if(!child_pid) return;
   
   if(kill(child_pid,sig) < 0){

         perror("Error Sending Sigint to child\n");


  }


}

int main(){

 char cmd[MAX_CMD];
 int argc;
 char *argv[MAX_ARGV];

 signal(SIGINT, sig_handler);    
 
 
 while(1){

  printf(PROMPT);

    if(fgets(cmd, MAX_CMD, stdin) == NULL){
        perror("fgets error");
        exit(1);
    }

        // ctrl + D to exit / end of file signal
    if(feof(stdin)){
            exit(0);
    }

     argc = 0;
     // first token:
     argv[argc] = strtok(cmd,SEP);

     while(argv[argc]!=NULL){

        argv[++argc] = strtok(NULL,SEP);

    }
   
    if(argc == 0){

          continue;

    }
    // builtin shell command help and quit:
    if(strcmp(argv[0], "help") == 0){

         printf("Welcome to my shell, builtins are quit and help\n");

         continue; 

    }

    if(strcmp(argv[0],"quit") == 0){

      exit(1);

    }

    // using the shell command

    

    child_pid = fork();

    if(child_pid < 0){
            perror("Fork Error");

            exit(1);

    }

    if(child_pid == 0){
        // child process
       if(execvp(argv[0],argv) < 0){
          
                perror("execvp error");
                exit(1);
           

        }
        exit(1);


    }

    int status;

    waitpid(child_pid, &status,0);
    
}

 return 0;

}
