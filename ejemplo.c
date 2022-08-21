#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

main()
{
    int a, b, c;
    int pid;
    int pipefd[2];
    char buff[10];
    char str[100 + 1] = {'\0'};
    int w, r;
    a = 3;
    b = 10;

    pipe(pipefd);
    if (pipefd < 0)
        printf("pipe error\n");

    pid = fork();
    if (pid == -1)
        printf("Fork error\n");
    if (pid == 0)
    {
        // child
        c = a * b;
        printf("child says answer %d\n", c);

        sprintf(str, "%d", c);
        w = write(pipefd[1], str, c);
        if (w != c)
            printf("write error");
        exit(0);
    }
    if (pid > 0)
    {
        // parent
        printf("father says answer %d\n", c);
        wait(0);

        r = read(pipefd[0], buff, sizeof(buff));
        if (r <= 0)
            printf("read error\n");
        c = atoi(buff);
        printf("child told father answer is %d\n", c);
    }
}