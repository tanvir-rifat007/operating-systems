#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROMPT 1024
#define PROMPT "\033[1;34m\xf0\x9f\x89\x80\033[0m "
#define MAX_CMDS 8
#define MAX_JOBS 8
#define MAX_ARGV 256
#define SEP " \t\n"
#define STATE_DEFAULT = 0
#define STATE_STOPPED = 1
#define STATE_FG = 2
#define STATE_BG = 3

volatile pid_t child_pid = 0;

void sigint_handler(int sig) {

  if (!child_pid)
    return;

  if (kill(child_pid, sig) < 0) {

    perror("Error Sending Sigint to child\n");
  }
  return;
}

int main() {

  char prompt[MAX_PROMPT]; // prompt means whole line, ex: ls -lsah or cat
  int argi, cmdi;
  char *cmds[MAX_CMDS][MAX_ARGV];
  char *tok;

  signal(SIGINT, sigint_handler);

  while (1) {

    printf(PROMPT);

    if (fgets(prompt, MAX_PROMPT, stdin) == NULL) {
      perror("fgets error");
      exit(1);
    }

    // ctrl + D to exit / end of file signal
    if (feof(stdin)) {
      exit(0);
    }

    argi = 0;
    cmdi = 0;
    // first token:
    tok = strtok(prompt, SEP);

    while (1) {
      cmds[cmdi][argi++] = tok;

      if (tok == NULL)
        break;

      tok = strtok(NULL, SEP);

      if (tok && strcmp(tok, "|") == 0) {
        cmds[cmdi++][argi] = NULL;
        argi = 0;
        tok = strtok(NULL, SEP);
      }
    }

    if (cmds[0][0] == 0) {

      continue;
    }
    // builtin shell command help and quit:
    if (strcmp(cmds[0][0], "help") == 0) {
      printf("\033[1;32mWelcome to \033[1;36mTANVIR'S SHELL\033[0m, "
             "\033[1;33mbuiltins\033[0m are \033[1;32mquit\033[0m, "
             "\033[1;32mhelp\033[0m, \033[1;32msearchyt <your song "
             "name>\033[0m\n");

      continue;
    }

    if (strcmp(cmds[0][0], "quit") == 0) {

      exit(1);
    }

    if (strcmp(cmds[0][0], "searchyt") == 0 && cmds[0][1] != NULL) {
      char query[512] = {0};
      for (int i = 1; i < cmds[0][i]; ++i) {
        strcat(query, cmds[0][i]);
        if (cmds[0][i + 1] != NULL)
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

    int fds[2];
    int infd = 0;
    int childpids[cmdi + 1];

    for (int i = 0; i <= cmdi; i++) {
      if (i != cmdi) {
        pipe(fds);
      }
      if ((childpids[i] = fork()) < 0) {
        perror("fork error");
        exit(1);
      }
      // child process:
      if (childpids[i] == 0) {

        if (i != cmdi) {
          dup2(fds[1], 1); // duplicate write end

          close(fds[1]);

          close(fds[0]);
        } // this if for ls
        dup2(infd, 0); // this is for wc

        if (execvp(cmds[i][0], cmds[i]) < 0) {

          perror("exec error");
          printf("\033[1;32mType 'help' to see all available builtin "
                 "commands\033[0m\n");

          exit(1);
        }
        exit(1);
      }
      // parent process:

      infd = fds[0];
      if (i != cmdi) {

        close(fds[1]);
      }
      int status;
      // parent process is beind waited to completed the all child's processes
      if (waitpid(childpids[i], &status, 0) < 0) {
        perror("waitpid error");
      }
    }
  }
  return 0;
}
