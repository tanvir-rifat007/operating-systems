#include<stdio.h>
#include<stdlib.h>
#include<signal.h>


void handler(int sig){
    printf("Signal %d received\n", sig);
    exit(0);
}



int main(){

 signal(SIGINT, handler);

    printf("Press Ctrl+C to send SIGINT signal\n"); 

  
    while(1) {
        // Infinite loop to keep the program running
    }
 

 return 0;





}
