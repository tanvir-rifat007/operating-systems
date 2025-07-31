#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define LOOP 100000
volatile int counter = 0;

pthread_mutex_t lock;

void *thread_entry(void *arg) {
  for (int i = 0; i < LOOP; i++) {
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
  }

  return NULL;
}

int main() {
  pthread_t p1, p2;
  pthread_mutex_init(&lock, NULL);
  pthread_create(&p1, NULL, thread_entry, NULL);

  pthread_create(&p2, NULL, thread_entry, NULL);
  pthread_join(p1, NULL);

  pthread_join(p2, NULL);

  pthread_mutex_destroy(&lock);
  printf("The result is : %d\n", counter);
  return 0;
}
