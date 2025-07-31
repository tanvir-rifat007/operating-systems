#include <stdio.h>
#include <stdlib.h>

void array(int *arr) {

  for (int i = 0; i < 1; i++) {

    printf("%d\n", *arr);
  }
}

int main() {

  int arr[5] = {10, 2, 3, 5, 6};
  array(arr + 3);
  return 0;
}
