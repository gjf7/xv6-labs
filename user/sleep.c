#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "error: argument expected\n");
        exit(1);
    }
    
    int tick_count = atoi(argv[1]);
    
    if (sleep(tick_count) == -1) {
        fprintf(2, "system call error\n");
        exit(1);
    }

    return 0;
}



