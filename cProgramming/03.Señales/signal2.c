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
        printf("HIJO: Recibida senial %d\n", sig); // printf dentro de un handler es mala idea. NO es async-signal-safe (comportamientos raros en programas reales)
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
        //while(1); mejor hacer pause();
        pause(); // Duerme hasta que llegue una senial.
    }
    else // PADRE
    {
        kill(pid, SIGUSR1);
        wait(NULL); // SI no se hace el wait, el hijo seria un zombie.
        printf("El hijo termino\n");
    }
    return 0;
}