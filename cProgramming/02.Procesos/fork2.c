#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    int estado;
    pid_t pid_wait;

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
        pid_wait = wait(&estado);

        printf("\n\t --- Mi PID es %d, Soy el padre de %d\n", getpid(), pid);
        printf("\n\t --- Numero: %f\n", numero);
        printf("\nestado: %d\n", estado);
        printf("pid_wait: %d\n", pid_wait);
        fflush(stdout);
    }
    else
    {  // HIJO
        numero = 25;

        printf("\n\t ~~~ Mi PID es %d, Soy el hijo de %d\n", getpid(), getppid());
        printf("\n\t ~~~ Numero: %f\n", numero);
        fflush(stdout);
    }
}