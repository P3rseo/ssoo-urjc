#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    int pipe_des[2], pid;
    char buf[1024];
    FILE *fd;
    pipe(pipe_des);
    pid = fork();
    if(pid == 0) 
    { // Hijo
        close(pipe_des[1]); //El hijo solo recibe, cierro el pipe[1]
        fd = fdopen(pipe_des[0], "r"); // Tenemos el FILE * con el descriptor del pipe
        fgets(buf, 1024, fd);
        fclose(fd);
        printf("Hijo: Recibido el siguiente mensaje: \"%s\"\n", buf);
    }
    else 
    { // Padre
        close(pipe_des[0]); //El padre solo envía, cierro el pipe[0]
        fd = fdopen(pipe_des[1], "w"); // Tenemos el FILE * con el descriptor del pipe
        fprintf(fd,"Hola hijo");
        fflush(fd);
        fclose(fd);
        wait(NULL);
        puts("Padre: El hijo terminó");
    }
    exit(0);
}