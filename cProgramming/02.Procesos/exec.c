#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    pid = fork();
    int status;

    if (pid < 0)  // ERROR
    {
        fprintf(stderr, "Fallo el fork()\n");
        exit(-1);
    }
    else if (pid == 0)
    {
        printf("Hola, soy el hijo\n");
        execlp("ls", "pepito", "-la", NULL);
        fprintf(stderr, "Se ha producido un error\n");
        exit(1);
    }
    else
    {
        wait(&status);
        printf("El hijo ha terminado\n");
        exit(0);
    }
}