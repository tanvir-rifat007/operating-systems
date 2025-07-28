#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  char str[] = "My name is Tanvir,Rifat";

  char *token = strtok(str, " ,");

  while (token != NULL) {

    printf("Token %s\n", token);
    token = strtok(NULL, " ,");
  }

  return 0;
}
