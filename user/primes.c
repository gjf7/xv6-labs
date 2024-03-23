#include "kernel/types.h"
#include "user/user.h"


void new_process(int *p) {
    close(p[1]);

    int prime;
    if (read(p[0], &prime, 4) != 4) {
        fprintf(2, "child process read error\n");
        exit(1);
    }
    fprintf(1, "prime %d\n", prime);
    
    int n;
    if (read(p[0], &n, 4)) {
        int new_p[2];
        pipe(new_p);

        if (fork() == 0) {
            new_process(new_p);
        } else {
            close(new_p[0]);
            if (n % prime != 0) write(new_p[1], &n, 4);

            while (read(p[0], &n, 4)) {
                if (n % prime != 0) write(new_p[1], &n, 4);
            }
            close(p[0]);
            close(new_p[1]);
            wait(0);
        };
        exit(0);
    };
}

int main() {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        new_process(p);
    } else {
        close(p[0]);
        for (int n = 2; n <= 35; n++) {
            if (write(p[1], &n, 4) != 4) {
                fprintf(2, "write error\n");
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
    return 0;
}
