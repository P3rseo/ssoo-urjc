#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int N = 10;

    // 1) Comprobacion de argumentos
    if (argc > 2) {
        fprintf(stderr, "Uso: %s [-N]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {

        if (argv[1][0] != '-') {
            fprintf(stderr, "Uso: %s [-N]\n", argv[0]);
            return 2;
        }

        N = atoi(argv[1] + 1); // Introducimos en la terminal -2, -5, ...
        if (N < 0) N = -N; // Por si ponen un numero negativo -(-6)
    }

    // 2) Leer N lineas de stdin
    char buf[1024];
    int count = 0;

    while (count < N && fgets(buf, sizeof(buf), stdin) != NULL) {
        fputs(buf, stdout);
        count++;
    }

    return 0;
}