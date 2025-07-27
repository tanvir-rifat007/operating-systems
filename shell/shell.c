#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CMD 1024
#define PROMPT "\xf0\x9f\x89\x80 "

#define MAX_ARGV 256
#define SEP " \t\n"
volatile pid_t child_pid = 0;

void sig_handler(int sig) {

  if (!child_pid)
    return;

  if (kill(child_pid, sig) < 0) {

    perror("Error Sending Sigint to child\n");
  }
}

int main() {

  char prompt[MAX_CMD]; // prompt means whole line, ex: ls -lsah or cat foo.txt
  int argc;
  char *argv[MAX_ARGV];

  signal(SIGINT, sig_handler);

  while (1) {

    printf(PROMPT);

    if (fgets(prompt, MAX_CMD, stdin) == NULL) {
      perror("fgets error");
      exit(1);
    }

    // ctrl + D to exit / end of file signal
    if (feof(stdin)) {
      exit(0);
    }

    argc = 0;
    // first token:
    argv[argc] = strtok(prompt, SEP);

    while (argv[argc] != NULL) {

      argv[++argc] = strtok(NULL, SEP);
    }

    if (argc == 0) {

      continue;
    }
    // builtin shell command help and quit:
    if (strcmp(argv[0], "help") == 0) {

      printf("Welcome to my shell, builtins are quit,help,searchyt <your song "
             "name>\n");

      continue;
    }

    if (strcmp(argv[0], "quit") == 0) {

      exit(1);
    }

    if (strcmp(argv[0], "searchyt") == 0 && argc >= 2) {
      char query[512] = {0};
      for (int i = 1; i < argc; ++i) {
        strcat(query, argv[i]);
        if (i != argc - 1)
          strcat(query, " ");
      }

      char full_query[600];
      snprintf(full_query, sizeof(full_query), "ytdl://ytsearch1:%s", query);

      child_pid = fork();
      if (child_pid < 0) {
        perror("Fork error");
        continue;
      }

      if (child_pid == 0) {
        // you have to install mpv in your machine first
        execlp("mpv", "mpv", "--no-video", full_query, (char *)NULL);
        perror("mpv failed");
        exit(1);
      }

      waitpid(child_pid, NULL, 0);
      child_pid = 0;
      continue;
    }
    // using the shell command

    child_pid = fork();

    if (child_pid < 0) {
      perror("Fork Error");

      exit(1);
    }

    if (child_pid == 0) {
      // child process
      if (execvp(argv[0], argv) < 0) {

        perror("execvp error");
        printf("Type \"help\" to see the builtin commands\n");
        exit(1);
      }
      exit(1);
    }

    int status;

    waitpid(child_pid, &status, 0);
  }

  return 0;
}
