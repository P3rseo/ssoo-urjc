#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    int fd[2]; // fd[0]=lectura, fd[1]=escritura

    // 1) Crear pipe
    if (pipe(fd) == -1) die("pipe");

    // 2) Crear hijo 1 -> productor: ls -1 (escribe al pipe)
    pid_t p1 = fork();
    if (p1 == -1) die("fork p1");

    if (p1 == 0) {
        // Redirigir stdout -> fd[1]
        if (dup2(fd[1], STDOUT_FILENO) == -1) die("dup2 p1 stdout");

        // Cerrar extremos del pipe que ya no necesito
        close(fd[0]);
        close(fd[1]);

        // Ejecutar ls -1
        execlp("ls", "ls", "-la", (char *)NULL);

        // Si llega aquí, exec falló
        die("execlp ls");
    }

    // 3) Crear hijo 2 -> consumidor: wc -l (lee del pipe)
    pid_t p2 = fork();
    if (p2 == -1) die("fork p2");

    if (p2 == 0) {
        // Redirigir stdin <- fd[0]
        if (dup2(fd[0], STDIN_FILENO) == -1) die("dup2 p2 stdin");

        // Cerrar extremos del pipe que ya no necesito
        close(fd[0]);
        close(fd[1]);

        // Ejecutar wc -l
        execlp("tr", "tr", "d", "D" , (char *)NULL);

        // Si llega aquí, exec falló
        die("execlp wc");
    }

    // 4) Padre: cierra el pipe (no lo usa) y espera a ambos hijos
    close(fd[0]);
    close(fd[1]);

    int status;
    if (waitpid(p1, &status, 0) == -1) die("waitpid p1");
    if (waitpid(p2, &status, 0) == -1) die("waitpid p2");

    return 0;
}
