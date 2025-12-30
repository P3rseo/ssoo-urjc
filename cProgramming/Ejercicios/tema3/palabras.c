#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXWORD 256

static int ends_with_txt(const char *s) {
    size_t n = strlen(s);
    return (n >= 4 && strcmp(s + (n - 4), ".txt") == 0);
}

static void bubble_sort(char **a, int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (strcmp(a[j], a[j + 1]) > 0) {
                char *tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s fichero.txt num_palabras\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    if (!ends_with_txt(filename)) {
        fprintf(stderr, "Error: el fichero debe tener extension .txt\n");
        return 1;
    }

    char *endptr = NULL;
    long Nl = strtol(argv[2], &endptr, 10);
    if (*argv[2] == '\0' || *endptr != '\0' || Nl <= 0 || Nl > 1000000) {
        fprintf(stderr, "Error: num_palabras debe ser un entero positivo\n");
        return 1;
    }
    int N = (int)Nl;

    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error abriendo '%s': %s\n", filename, strerror(errno));
        return 1;
    }

    // Reservamos array de punteros
    char **words = (char **)malloc((size_t)N * sizeof(char *));
    if (!words) {
        perror("malloc");
        fclose(f);
        return 1;
    }

    // Reservamos cada palabra (tamaño fijo MAXWORD)
    for (int i = 0; i < N; i++) {
        words[i] = (char *)malloc(MAXWORD);
        if (!words[i]) {
            perror("malloc");
            for (int k = 0; k < i; k++) free(words[k]);
            free(words);
            fclose(f);
            return 1;
        }
        words[i][0] = '\0';
    }

    // Leemos N palabras
    int read_count = 0;
    char buf[MAXWORD];

    while (read_count < N && fscanf(f, "%255s", buf) == 1) {
        strcpy(words[read_count], buf);
        read_count++;
    }

    fclose(f);

    if (read_count < N) {
        fprintf(stderr, "Aviso: el fichero tiene menos palabras (%d) que N (%d).\n",
                read_count, N);
        // Ajustamos N al número real leído
        N = read_count;
    }

    bubble_sort(words, N);

    for (int i = 0; i < N; i++) {
        puts(words[i]);
    }

    for (int i = 0; i < (int)Nl; i++) {  // liberamos las que reservamos
        free(words[i]);
    }
    free(words);

    return 0;
}
