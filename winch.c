#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void handler(int sig){

 printf("Terminal resized\n");


}



int main(){

   struct sigaction sa;

    sa.sa_handler = handler;

    sigaction(SIGWINCH, &sa, NULL);


    while(1){

        // Wait for the signal
        pause();

        printf("Signal received, handling...\n");


    }    
    



}
