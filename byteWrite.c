#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>


#define ONE_MEGABYTE 1048576



int main(){
 int fd = open("/tmp/foo", O_WRONLY | O_TRUNC | O_CREAT, 0644);

 if (fd < 0) {
    perror("open");
    exit(EXIT_FAILURE);
 
 }

 int prior_blocks = -1;

 struct stat st;


for(int i = 0; i < ONE_MEGABYTE; i++){

   write(fd, "A", 1);
   fstat(fd, &st);
    if (st.st_blocks != prior_blocks) {

        printf("size : %d bytes, blocks : %lld\n, on disk : %lld\n", 
               (int)st.st_size, st.st_blocks, st.st_blocks * 512);
        prior_blocks = st.st_blocks;
    }

}



 


 return 0;

}
