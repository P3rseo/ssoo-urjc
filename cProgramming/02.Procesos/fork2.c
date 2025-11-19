#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    float numero = 123.543;
    pid_t pid;
    pid = fork();

    if(pid == -1)
    {
        perror("Error al crear un nuevo Hijo\n");
        return(1);
    }

    if(pid)  // PADRE
    {
        printf("\n\t --- Mi PID es %d, Soy el padre de %d\n", getpid(), pid);
        printf("\n\t --- Mi Padre es %d.\n", getppid());
        printf("\n\t --- Numero: %f\n", numero);
        fflush(stdout);
    }
    else
    {  // HIJO
        printf("\n\t ~~~ Mi PID es %d, Soy el hijo de %d\n", getpid(), getppid());
        printf("\n\t ~~~ Numero: %f\n", numero);
        fflush(stdout);
    }
}