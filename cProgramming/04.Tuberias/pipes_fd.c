#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main() 
{
    int pipe_des[2], pid;
    char buf[1024];
    pipe(pipe_des);
    pid = fork();
    if(pid == 0) 
    { // Hijo
        puts("Hijo: Hola, soy el hijo");
        close(pipe_des[1]); //El hijo solo recibe, cierro el pipe[1]
        read(pipe_des[0], buf, 1024);
        close(pipe_des[0]);
        printf("Hijo: Recibido el siguiente mensaje: \"%s\"\n", buf);
    }
    else 
    { // Padre
        puts("Padre: Hola, soy el padre");
        close(pipe_des[0]); //El padre solo envía, cierro el pipe[0]
        write(pipe_des[1], "Hola hijo", 10);
        close(pipe_des[1]);
        wait(NULL);
        puts("Padre: El hijo terminó");
    }
    exit(0);
}