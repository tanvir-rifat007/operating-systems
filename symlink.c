#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>


int main(int argc, char *argv[]){

 if(argc < 3){
          fprintf(stderr, "Usage: %s <target> <linkname>\n", argv[0]);
        return 1;


}

 const char *target = argv[1];
 const char *linkname = argv[2];


 if(symlink(target,linkname) == -1){



    perror("symlink failed");

    return 1;

}

 printf("Symbolink is created %s -> %s\n",linkname,target);
 


 return 0;

}
