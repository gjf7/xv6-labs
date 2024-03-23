#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pid;
    int p[2];
    char buf;
    
    pipe(p);
    pid = fork();
    if (pid > 0) {
        write(p[1], "c", 1);
        close(p[1]);
        
        if (read(p[0], &buf, 1) == 1) {
            fprintf(1, "%d: received pong\n", getpid());
            close(p[0]);
            exit(0);
        }
        exit(1);
    } else if (pid == 0) {
        if (read(p[0], &buf, 1) == 1) {
            fprintf(1, "%d: received ping\n", getpid());
            close(p[0]);
            write(p[1], "c", 1);
            close(p[1]);
            exit(0);
        }
        exit(1);
    } else {
      fprintf(2, "fork error");
      exit(1);
    }
    return 0;
}
    

