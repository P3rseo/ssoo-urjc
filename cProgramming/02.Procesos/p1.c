#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() 
{
    pid_t pid;
    pid = fork();
    if (pid < 0) 
    { /* Error */
        fprintf(stderr, "Falló el fork()");
        exit(-1);
    }
    else if (pid == 0) 
    { /* Proceso Hijo */
        printf("Hola, soy el hijo.\n");
        sleep(10);
        exit(0);
    }
    else 
    { /* Proceso Padre */
        waitpid (pid, NULL, 0);
        printf ("El hijo terminó.\n");
        exit(0);
    }
}