#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

// exceclp: BUSCA EL EJECUTABLE EN EL PATH (COMO LA SHELL).
// execv: NO BUSCA EN PATH, LE PASAS LA RUTA EXACTA.

int main()
{
    pid_t pid1;
    pid1 = fork();

    if (pid1 < 0)  // ERROR
    {
        fprintf(stderr, "Fallo el fork()\n");
        exit(-1);
    }
    else if (pid1 == 0)
    {
        printf("Hola, soy el hijo\n");
        execlp("ls", "pepito", "-l", "-a", NULL); // execlp p: path (<nombre del programa a ejecutar>, <arg0>, <argumentos del comando 1>, <args comando 2>, ..., <NULL marca final>)
        execl("/bin/ls", "ls", "-l", "-a", NULL);
        fprintf(stderr, "Se ha producido un error\n");
        exit(1);
    }
    else
    {
        wait(NULL);
        printf("El hijo ha terminado\n");
        //exit(0); quiero continuar con otros ejemplos
    }

    pid_t pid2;
    pid2 = fork();
    char *argv[] = { "ls", "-l", "-a", NULL };

    if (pid2 < 0)  // ERROR
    {
        fprintf(stderr, "Fallo el fork()\n");
        exit(-1);
    }
    else if (pid2 == 0)
    {
        printf("Hola, soy el hijo\n");
        execv("/bin/ls", argv);
        execvp("ls", argv);
        perror("execv");
        exit(1);
    }
    else
    {
        wait(NULL);
        printf("El hijo ha terminado\n");
        exit(0);
    }
}