#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// WIFEXITED: SABER SI UN HIJO TERMINO "NORMAL" (CON EXIT / RETURN).
// WEXITSTATUS: SOLO SE PUEDE UTILIZAR CUANDO WIFEXITED ES VERDADERO (TERMINO NORMAL) Y NOS DA SU CODIGO DE SALIDA.

int main(void) {
    pid_t pid = fork();
    printf("Mi PID es: %d\n", getpid());
    if ( pid == 0) exit(3);

    int status;
    int ended;
    //while ((ended = wait(&status)) > 0) {
    //printf("He recogido al hijo %d\n", ended);
    //}
    ended = wait(&status);

    if (WIFEXITED(status)) {
        printf("El hijo con pid %d terminó normalmente. Y mi pid = %d. Y mi status = %d\n", ended, getpid(), WEXITSTATUS(status));
    } else {
        printf("El hijo NO terminó normalmente (señal, etc.).%d\n", getpid());
    }

    printf("Mi PID es: %d\n", getpid());
    return 0;
}
