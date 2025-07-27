#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>




int main(){

  pid_t pid = fork();

  if(pid < 0){

    perror("Fork failed");
    exit(EXIT_FAILURE);

    }else if(pid == 0){

    // Child process

        execl("/bin/ls", "ls", "-lsah", NULL);

        printf("Ami print hobo nah karon execl call korar por process ta replace hoye jabe\n");


    }else{
      // parent process:

        wait(NULL);
        printf("Child process completed\n");
        printf("Parent process exiting\n");
        exit(EXIT_SUCCESS);



    }



  return 0;


}
