#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024

int main(int argc, char* argv[]) {
    
    int N = 10;  // Valor por defecto de N

    // 1) Comprobacion de argumentos.
    if (argc > 2) {
        fprintf(stderr, "Uso: %s [-N]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
        if (argv[1][0] != '-') {
            fprintf(stderr, "Uso: %s [-N]\n", argv[0]);
            return 2;
        }

        N = atoi(argv[1] + 1); // Pasamos de string a int
        if (N < 0) N = -N; // Por si introducen un numero negativo -(-5)
    }

    // 2) Leer N lineas.
    char (*lines)[MAXLINE] = malloc((size_t)N * MAXLINE);
    if (!lines) {
        perror("malloc");
        return 3;
    }

    char buf[MAXLINE];
    long count = 0;

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        int pos = (int)(count % N);
        strcpy(lines[pos], buf);
        count++;
    }

    int total = (count < N) ? (int)count : N;
    int start = (count < N) ? 0 : (int)(count % N);

    for (int i = 0; i < total; i++) {
        int idx = (start + i) % N;
        fputs(lines[idx], stdout);
    }
    printf("\n");
    free(lines);
    return 0;
}