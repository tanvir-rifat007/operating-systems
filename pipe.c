// ls | wc

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main(){


 int fds[2];  // fd[0] = means read end and fd[1] = means write end

 pid_t lsPid, wcPid;

 if(pipe(fds) == -1){
        perror("Pipe error");
        exit(1);

 }


 lsPid = fork();

 if(lsPid == -1){

        perror("lsPid error");
        exit(1);



}

 if(lsPid == 0){

   //child process:
    close(fds[0]); // fds[0] = means read int and we should close the read int in the ls 

    dup2(fds[1],STDOUT_FILENO); // ls gives stdout to the wc

    close(fds[1]);
    execlp("ls","ls",NULL);

    perror("Execlp error");

    exit(1);


}

wcPid = fork();

if(wcPid == -1){
  perror("wc error"); 
  exit(1);


}
if(wcPid == 0){
  close(fds[1]);

  dup2(fds[0],STDIN_FILENO);

  close(fds[0]);
  
  execlp("wc", "wc", NULL);

  perror("Execlp error");

  exit(1);

}

 close(fds[0]);
 close(fds[1]);

 waitpid(lsPid, NULL, 0);

 waitpid(wcPid, NULL, 0);

  return 0;

}
