#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int readline(int argc, char *argv[], char *buf) {
  int n = 0;
  while (read(0, buf+n, 1)) {
    if (n == 1023) {
      fprintf(2, "xargs: argument is too long\n");
      exit(1);
    }

    if (buf[n] == '\n') break;

    n++;
  }
  buf[n] = 0;
  if (n == 0) return 0;
  
  int offset = 0;
  while (offset < n) {
    argv[argc++] = buf + offset;

    while(buf[offset] != ' ' && offset < n) {
      offset++;
    }

    while(buf[offset] == ' ' && offset < n) {
      buf[offset++] = 0;
    }
  }

  argv[argc] = 0;

  return argc;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "Usage: xargs <command>\n");
    exit(1);
  }

  char *command = argv[1];
  char *full_argv[MAXARG];
  full_argv[0] = command;
  

  int new_argc = 1;
  for (int i = 2; i < argc && new_argc < MAXARG; i++, new_argc++) {
    full_argv[new_argc] = argv[i];
  }

  char buf[1024];
  while (readline(new_argc, full_argv, buf)) {
    if (fork() == 0) {
      exec(command, full_argv);
      fprintf(2, "exec %s failed\n", command);
      exit(1);
    }
    wait(0);
  }


  return 0;
}
