#include <stdio.h>
#include <stdlib.h>

typedef int job;

typedef struct {
  job *buffer; // we work with the int type of job here
  int capacity;
  int size;
  int wp; // location at which to write
  int rp; // location at where to read
} JOBQUEUE;

JOBQUEUE *JQ_init(int cap) {
  JOBQUEUE *j = malloc(sizeof(JOBQUEUE));
  j->buffer = malloc(cap * sizeof(job));
  j->capacity = cap;
  j->size = 0;
  j->wp = 0;
  j->rp = 0;
  return j;
}

void JQ_free(JOBQUEUE *jq) {

  free(jq->buffer);
  free(jq);
}
void JQ_add(JOBQUEUE *jq, job item) {
  // TODO: check if the queue is full

  jq->buffer[jq->wp] = item;
  printf("%d -> [%d]\n", item, jq->wp);
  jq->wp = (jq->wp + 1) % jq->capacity; // ring buffer formula
}

void JQ_get(JOBQUEUE *jq) {
  // TODO: check if the queue is full

  job item = jq->buffer[jq->rp];
  printf("     [%d] -> %d\n", jq->rp, item);
  jq->rp = (jq->rp + 1) % jq->capacity; // ring buffer formula
}

int main() {
  // capacity of 8 elements jobqueue
  JOBQUEUE *jq = JQ_init(8);
  int i;
  for (i = 0; i < 5; i++)
    JQ_add(jq, i);
  for (i = 0; i < 5; i++)
    JQ_get(jq);
  for (i = 0; i < 5; i++)
    JQ_add(jq, i);
  for (i = 0; i < 5; i++)
    JQ_get(jq);

  JQ_free(jq);
}
