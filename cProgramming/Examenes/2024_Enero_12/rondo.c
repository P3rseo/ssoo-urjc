#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define N 4

int main() {
    
    pid_t pid[N];
    int pipes[N][2];

    // Inicializar pipes
    for (int i = 0; i < N; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipes");
            return 1;
        }
    }

    for (int i = 0; i < N; i++) {
        pid[i] = fork();

        if (pid[i] == 0) { // Proceso Hijo (Jugador)
            
            // Lee del Jugador anterior
            int prev = (i + N - 1) % N;
            int fd_in = pipes[prev][0];
            
            // Escribe en el siguiente
            int fd_out = pipes[i][1];

            // Cerramos todos los pipes que no necesitamos
            for (int k = 0; k < N; k++) {
                if (pipes[k][0] != fd_in) {
                    close(pipes[k][0]);
                }
                if (pipes[k][1] != fd_out) {
                    close(pipes[k][1]);
                }
            }

            // Leer 1 byte y pasarlo
            char ball;
            while(read(fd_in, &ball, 1) == 1) {
                printf("Jugador %d recibe bola\n", i);
                fflush(stdout);
                sleep(1);
                write(fd_out, &ball, 1);
            }
            
            close(fd_in);
            close(fd_out);
            _exit(0);
        }
    }

    // Cerramos todos los pipes del padre menos donde metemos bola
    for (int i = 0; i < N; i++) {
        close(pipes[i][0]);
        if (i != (N-1)) {
            close(pipes[i][1]);
        }
    }

    char ball = 'B';
    write(pipes[N-1][1], &ball, 1);
    close(pipes[N-1][1]);

    sleep(10);

    return 0;
}