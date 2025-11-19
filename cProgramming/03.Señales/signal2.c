# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

int pid;

void manejador(int sig)
{
    if(pid == 0) // HIJO
    {
        printf("HIJO: Recibida senial %d\n", sig);
        kill(getppid(), SIGUSR1);
        exit(0);
    }
    else
    {
        printf("PADRE: Recibida senial %d\n", sig);
    }
}

int main()
{
    signal(SIGUSR1, manejador);
    pid = fork();
    if(pid == 0) // HIJO
    {
        while(1);
    }
    else // PADRE
    {
        kill(pid, SIGUSR1);
        wait(NULL);
        printf("El hijo termino\n");
    }
    return 0;
}