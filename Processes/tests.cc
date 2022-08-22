#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid;

    // You can take number of child process to be create as input from user

    for (int i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0) {
            // printf("[child] pid %d from [parent] pid %d\n", getpid(), getppid());
            break;
        }
    }

    if (pid != 0) {
        int cpid;
        int status;
        while ((cpid = wait(&status)) > 0)
            // printf("Parent (%d): child (%d) exited with status 0x%.4x\n", (int)getpid(), cpid, status);
            ;
    }

    return 0;
}