#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    
    pid_t pid;
    int p_padre[2]; // Escribe el padre
    int p_hijo[2]; // Escribe el hijo
    char buf[1024];
    
    FILE *p_p, *p_h;
    
    // Comprobacion de argumentos
    if (argc > 1) {
        fprintf(stderr, "Uso: %s\n", argv[0]);
        return 1;
    }

    if (pipe(p_padre) == -1) { /* Error pipe */
        perror("pipe1");
        return 2;
    }

    if (pipe(p_hijo) == -1) { /* Error pipe */
        perror("pipe2");
        return 3;
    }

    pid = fork();
    if (pid < 0) { /* Error fork */
        perror("fork");
        return 4;
    }
    else if (pid == 0) { /* Proceso Hijo */
        close(p_padre[1]);
        close(p_hijo[0]);
        p_p = fdopen(p_padre[0], "r"); // Abrimos lado lectura padre con permiso de lectura.
        p_h = fdopen(p_hijo[1], "w"); // Abrimos lado escritura hijo con permiso escritura.

        while (fgets(buf, 1024, p_p) != NULL) {
            int longitud = strlen(buf);
            for (int i = 0; i < longitud; i++) {
                buf[i] = toupper(buf[i]);
            }
            fputs(buf, p_h);
            fflush(p_h);
        }

        fclose(p_p);
        fclose(p_h);
        exit(0);
    }
    else { /* Proceso Padre */
        close(p_padre[0]);
        close(p_hijo[1]);
        p_p = fdopen(p_padre[1], "w");
        p_h = fdopen(p_hijo[0], "r");

        while (fgets(buf, 1024, stdin) != NULL) {
            fputs(buf, p_p);
            fflush(p_p);
            fgets(buf, 1024, p_h);
            fputs(buf, stdout);
        }

        fclose(p_p);
        fclose(p_h);
        wait(NULL);
    }

    return 0;
}