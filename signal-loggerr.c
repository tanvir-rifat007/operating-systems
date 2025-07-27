#include<stdio.h>
#include<stdlib.h>
#include <signal.h>
#include<unistd.h>
#include <setjmp.h>



volatile uint64_t handled = 0;
sigjmp_buf jmp_env_seg, jmp_env_bus;

void handle(int sig){
 handled |= (1 << sig);

 printf("Caught %d: %s (%d total)\n",sig,sys_siglist[sig],__builtin_popcount(handled));

  if(sig == SIGINT || sig == SIGTERM){
    printf("Exiting on signal %d\n", sig);
    exit(0);
  }

// Handle segmentation fault specifically

  if(sig == SIGSEGV){

    siglongjmp(jmp_env_seg, 1);

    }

// Handle bus error specifically
    
    if(sig == SIGBUS){

      siglongjmp(jmp_env_bus,1);


    }


}


int main(){
 
   for(int i = 0; i< NSIG; i++){

      signal(i,handle);


    }


    // segfault to test

   // int *p = NULL;
    // *p = 42; // This will cause a segmentation fault



   // recover the segfault

    int *p = NULL;
    if(sigsetjmp(jmp_env_seg,1) == 0){


       *p = 5;

    }


    // bus error to test
    int *q;

    if(sigsetjmp(jmp_env_bus,1) == 0){

       *q = 42;

    }

    while(1){
      sleep(1);

    }


    return 0;

}


