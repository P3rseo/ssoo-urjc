#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int p[2]; // Para almacenar los descriptores en la tuberia

// Manejador de seniales del hijo
// SIGUSR1 - Generar un numero aleatorio y ponerlo en la tuberia
// SIGUSR2 - Terminar el proceso hijo
void handler(int sig)
{
    int num;

    if(sig == SIGUSR2) // Terminar proceso
    {
        close(p[1]);
        exit(0);
    }
    else if (sig == SIGUSR1)
    {
        num = rand() % 11;
        write(p[1], &num, sizeof(int));
    }
}

int main(int argc, char* argv[]) {

    pid_t pid;

    char buf[1024];
    int num;

    // Activar el manejador de seniales
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    if (argc > 1) {
        fprintf(stderr, "Uso: %s\n", argv[0]);
        return 1;
    }

    if (pipe(p) == -1) { /* Error pipe padre */
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid < 0) { /* Error fork */
        perror("fork");
        return 1;
    }
    else if (pid == 0) { /* Proceso Hijo */
        close(p[0]);
        pause();
    }
    else { /* Proceso Padre */
        close(p[1]);
        while (fgets(buf, 1024, stdin) != NULL) {
            kill(pid, SIGUSR1);
            read(p[0], &num, sizeof(int));
            printf("%d\n", num);
        }
        close(p[0]);
        kill(pid, SIGUSR2);
        wait(NULL);
    }
    
    return 0;
}