// 15 and 0 are both fiz and buz, when we get in 15 contain's both fiz and buz
// means our solution is correct

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
volatile int n = 0, fiz = 0, buz = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void *fizz(void *args) {
  while (1) {

    pthread_mutex_lock(&lock);
    pthread_cond_wait(
        &cond,
        &lock); // when the parent thread or main thread change the value of n
                // then this fizz thread is work, otherwise it sleep.
    if (n % 3 == 0 && fiz == 0) {

      fiz = 1;
      printf("\tfizz");
    }
    pthread_mutex_unlock(&lock);
  }

  return NULL;
}

void *buzz(void *args) {
  while (1) {
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cond, &lock);
    if (n % 5 == 0 && buz == 0) {
      buz = 1;
      printf("\tbuzz");
    }
    pthread_mutex_unlock(&lock);
  }

  return NULL;
}

int main() {
  pthread_t t1, t2;

  pthread_create(&t1, NULL, fizz, NULL);

  pthread_create(&t2, NULL, buzz, NULL);

  // main thread writes to n;
  while (1) {
    pthread_mutex_lock(&lock);
    n = rand() & 0xf; // 0 - 15 random number
    fiz = 0;
    buz = 0;

    printf("\n%d : ", n);

    pthread_cond_broadcast(&cond);

    pthread_mutex_unlock(&lock);

    usleep(100000);
  }
  return 0;
}
