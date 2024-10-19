#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void print_process_info(int pid_flag, int ppid_flag, int uid_flag, int gid_flag) {
    if (pid_flag) {
        printf("PID: %d\n", getpid());  
    }
    if (ppid_flag) {
        printf("PPID: %d\n", getppid()); 
    }
    if (uid_flag) {
        printf("UID: %d\n", getuid());   
    }
    if (gid_flag) {
        printf("GID: %d\n", getgid());   
    }
}

int main(int argc, char *argv[]) {
    int pid_flag = 0, ppid_flag = 0, uid_flag = 0, gid_flag = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--pid") == 0) {
            pid_flag = 1;
        } else if (strcmp(argv[i], "--ppid") == 0) {
            ppid_flag = 1;
        } else if (strcmp(argv[i], "--uid") == 0) {
            uid_flag = 1;
        } else if (strcmp(argv[i], "--gid") == 0) {
            gid_flag = 1;
        }
    }

    print_process_info(pid_flag, ppid_flag, uid_flag, gid_flag);

    return 0;
}
