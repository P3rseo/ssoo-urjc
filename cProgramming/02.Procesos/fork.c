#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    int id;
    id = fork();

    if(id == 0)  // HIJO
    {
        printf("%s %d\n", "soy el hijo y mi id: ", getpid());
        sleep(10);
    }
    else
    {  // PADRE
        wait(NULL);
        printf("%s %d\n", "soy el padre y mi id: ", getpid());
    }
}