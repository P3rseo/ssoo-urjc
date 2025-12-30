#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int status;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s [comando] [args...]\n", argv[0]);
        return 1;
    }

    pid = fork();
    if (pid < 0) { /* Error */
        fprintf(stderr, "Error con el fork().\n%s\n", strerror(errno));
        return 1;
    }
    else if (pid == 0) { /* Proceso Hijo */
        execvp(argv[1], argv + 1);
        perror("Error al ejecutar el comando");
        //fprintf(stderr, "Error al ejecutar el comando:\n%s\n", strerror(errno));
        exit(1);
    }
    else { /* Proceso Padre */
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            return 1;
        }

        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            if (code != 0) {
                fprintf(stderr, "El comando no se ejecuto correctamente\n");
                return 1;
            }
            
        }
        exit(0);
    }

    return 0;
}