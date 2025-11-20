#include <stdio.h>
#include <unistd.h>

int main()
{
    int NUM = 3;
    pid_t pid = fork();
    for (int i = 0; i < NUM; i++)
    {
        if (pid != 0)  // THIS MEANS WE ARE IN THE FATHER.
        {
            // fork(); CAREFULL !! WE NEED TO ASIGN THE NEW PID.
            pid = fork();
        }
    } // This is gonna print NUM + 2 processes

    printf("Hello World from %d & Parent: %d\n", getpid(), getppid());
    return 0;
}