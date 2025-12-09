#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    pid = fork();

    if (pid < 0)  // ERROR
    {
        fprintf(stderr, "Fallo el fork()\n");
        exit(-1);
    }
    else if (pid == 0)
    {
        printf("Hola, soy el hijo\n");
        sleep(5);
        exit(0);
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("El hijo ha terminado\n");
        exit(0);
    }
}