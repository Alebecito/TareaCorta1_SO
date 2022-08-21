#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct result {
    int personId;
    int questionId;
    int choice;
};

int main(void)
{
    int p2[2];

    pipe(p2);

    if (fork() == 0)
    {
        close(p2[0]);
        struct result finalResults[3][20];
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                finalResults[i][j].personId = rand() % 36 + 100;
                finalResults[i][j].questionId = rand() % 20 + 1;
                finalResults[i][j].choice = rand() % 5 + 1;
            }
        }

        printf("Child:\n");
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                printf("(%d,%2d,%d)", finalResults[i][j].personId,
                       finalResults[i][j].questionId, finalResults[i][j].choice);
                if (j % 5 == 4)
                    putchar('\n');
            }
        }

        write(p2[1], &finalResults[0], sizeof(finalResults[0]));
        write(p2[1], &finalResults[1], sizeof(finalResults[1]));
        write(p2[1], &finalResults[2], sizeof(finalResults[2]));

        close(p2[1]);
    }
    else
    {
        struct result resultBuffer[1024];
        int n;
        close(p2[1]);
        while ((n = read(p2[0], resultBuffer, sizeof(resultBuffer))) > 0) {
            int n_entries = n / sizeof(resultBuffer[0]);
            printf("Read: %d entries\n", n_entries);
            for (int i = 0; i < n_entries; ++i) {
                if (resultBuffer[i].choice != 0)
                    printf("Result for question id: %2d, person id: %d, choice: %d\n",
                           resultBuffer[i].questionId, resultBuffer[i].personId,
                           resultBuffer[i].choice);
            }
        }
        close(p2[0]);
    }
    return 0;
}