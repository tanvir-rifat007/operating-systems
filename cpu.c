// this program is used to profile the CPU usage of a process
// it shows how much time the process spends in user mode, system mode, and real time
// it also illustrate how the os schedules the process on the CPU



#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/resource.h>
#include<stdint.h>
#include <cpuid.h>


#define SLEEP_SEC 10
#define NUM_MULS 100000000
#define NUM_MALLOCS 1000000
#define MALLOC_SIZE 1000
#define TOTAL_USEC(tv) ((tv).tv_sec * 1000000 + (tv).tv_usec)


unsigned getcpu(){

  unsigned regs[4];
  __cpuid_count(1,0,regs[0],regs[1],regs[2],regs[3]);

  return (regs[1] >> 24) ; // Extracting the CPU ID from the EBX register
}


struct profile_times{
  // here usec is microseconds
  // u means micro here(real_usec, user_usec, sys_usec)
 uint64_t  real_usec;
 uint64_t user_usec;
 uint64_t sys_usec;
};


void profile_start(struct profile_times *t){
    struct timeval tv;
    struct rusage ru;

    gettimeofday(&tv, NULL);
    getrusage(RUSAGE_SELF, &ru);

    t->real_usec = TOTAL_USEC(tv);     
    t->user_usec = TOTAL_USEC(ru.ru_utime); 
    t->sys_usec =  TOTAL_USEC(ru.ru_stime); 

}


void profile_log(struct profile_times *t){

   struct timeval tv;
   struct rusage ru;

   gettimeofday(&tv, NULL);
   getrusage(RUSAGE_SELF, &ru);


   fprintf(stderr, "[pid : %d], [cpu : %d], real: %.03f usec, user: %.03f usec, sys: %.03f usec\n",
           getpid(),
           getcpu(),
           (TOTAL_USEC(tv) - t->real_usec) / 1000000.0,
           (TOTAL_USEC(ru.ru_utime) - t->user_usec)/ 1000000.0,
           (TOTAL_USEC(ru.ru_stime) - t->sys_usec) / 1000000.0
            );

}


int main(int argc, char *argv[]) {
  struct profile_times t;

  float x = 1.0;
  profile_start(&t);
  for (int i = 0; i < NUM_MULS; i++)
    x *= 1.1;
  profile_log(&t);

  profile_start(&t);
  void *p;
  for (int i = 0; i < NUM_MALLOCS; i++)
    p = malloc(MALLOC_SIZE);
  profile_log(&t);


    free(p);


    profile_start(&t);
    for (int i =0; i < NUM_MALLOCS; i++){

       x+= 1.0;

    }
    profile_log(&t);



  profile_start(&t);
  sleep(SLEEP_SEC);
  profile_log(&t);
}



