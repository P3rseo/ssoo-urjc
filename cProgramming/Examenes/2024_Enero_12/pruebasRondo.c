#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 4

int main() {
    int p[N][2];

    // 1) Crear pipes
    for (int i = 0; i < N; i++) {
        if (pipe(p[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // 2) Crear jugadores
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(1); }

        if (pid == 0) {
            int id = i + 1;
            int prev = (i - 1 + N) % N;

            int fd_in  = p[prev][0]; // leer del anterior
            int fd_out = p[i][1];    // escribir al siguiente

            // 3) Cerrar TODO excepto fd_in y fd_out
            for (int k = 0; k < N; k++) {
                if (p[k][0] != fd_in)  close(p[k][0]);
                if (p[k][1] != fd_out) close(p[k][1]);
            }

            // Aquí el jugador ya tiene SOLO 2 FDs abiertos: fd_in y fd_out.
            // (Para la demo, imprimimos cuáles son)
            printf("Jugador %d: fd_in=%d, fd_out=%d\n", id, fd_in, fd_out);

            // Demo simple: leer 1 byte y pasarlo (loop)
            char ball;
            while (read(fd_in, &ball, 1) == 1) {
                printf("Jugador %d recibe\n", id);
                fflush(stdout);
                sleep(1);
                write(fd_out, &ball, 1);
            }

            close(fd_in);
            close(fd_out);
            _exit(0);
        }
        // padre sigue el bucle
    }

    // 4) Padre: NO participa en el rondo.
    // Cierra todos sus FDs salvo el que use para "arrancar" el balón.
    // Para que reciba primero Jugador 1, escribimos en p[3][1] (J4->J1).
    for (int i = 0; i < N; i++) {
        close(p[i][0]);               // padre no lee
        if (i != N - 1) close(p[i][1]); // cierra todas las escrituras menos p[3][1]
    }

    // 5) Iniciar el balón
    char ball = 'B';
    write(p[N-1][1], &ball, 1);
    close(p[N-1][1]); // padre ya no escribe más

    // 6) Esperar (en este demo se queda infinito; aquí esperamos un poco y terminamos)
    sleep(10);

    // En un ejercicio real, aquí mandarías señales para terminar ordenado, etc.

    return 0;
}
