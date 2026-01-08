// documentText.c
// Uso: documentText documento.txt palabra_busqueda fichero_salida
// - El padre lee el fichero y envía su contenido a dos hijos mediante pipes.
// - Hijo 1: imprime "palabra\t<apariciones>" usando grep -c.
// - Hijo 2: convierte todo a mayúsculas y lo guarda en fichero_salida.

#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

static void usage(const char *prog) {
    fprintf(stderr, "USO: %s documento.txt palabra_busqueda fichero_salida\n", prog);
    exit(EXIT_FAILURE);
}

static void close_checked(int fd) {
    if (close(fd) == -1) die("close");
}

int main(int argc, char *argv[]) {
    if (argc != 4) usage(argv[0]);

    const char *in_path = argv[1];
    const char *word = argv[2];
    const char *out_path = argv[3];

    // a) Comprobación apertura fichero de entrada
    int in_fd = open(in_path, O_RDONLY);
    if (in_fd == -1) {
        fprintf(stderr, "Error abriendo '%s': %s\n", in_path, strerror(errno));
        return EXIT_FAILURE;
    }

    int to_grep[2];
    int to_upper[2];

    if (pipe(to_grep) == -1) die("pipe to_grep");
    if (pipe(to_upper) == -1) die("pipe to_upper");

    // ---- Hijo 1: grep -c ----
    pid_t grep_pid = fork();
    if (grep_pid < 0) die("fork grep");

    if (grep_pid == 0) {
        // Cerrar extremos que no se usan
        close_checked(to_grep[1]);          // hijo lee
        close_checked(to_upper[0]);
        close_checked(to_upper[1]);
        close_checked(in_fd);

        // Redirigir stdin desde pipe
        if (dup2(to_grep[0], STDIN_FILENO) == -1) die("dup2 grep stdin");
        close_checked(to_grep[0]);

        // Escribir "palabra\t" en stdout y luego ejecutar grep -c palabra
        // (grep imprimirá el número y '\n')
        dprintf(STDOUT_FILENO, "%s\t", word);

        execlp("grep", "grep", "-c", word, (char *)NULL);
        // Si exec falla:
        die("execlp grep");
    }

    // ---- Hijo 2: convertir a mayúsculas y guardar ----
    pid_t upper_pid = fork();
    if (upper_pid < 0) die("fork upper");

    if (upper_pid == 0) {
        close_checked(to_upper[1]);         // hijo lee
        close_checked(to_grep[0]);
        close_checked(to_grep[1]);
        close_checked(in_fd);

        int out_fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1) {
            fprintf(stderr, "Error abriendo salida '%s': %s\n", out_path, strerror(errno));
            _exit(EXIT_FAILURE);
        }

        unsigned char buf[4096];
        for (;;) {
            ssize_t n = read(to_upper[0], buf, sizeof(buf));
            if (n == 0) break;
            if (n < 0) {
                if (errno == EINTR) continue;
                perror("read upper");
                break;
            }
            for (ssize_t i = 0; i < n; i++) {
                buf[i] = (unsigned char)toupper(buf[i]);
            }
            ssize_t off = 0;
            while (off < n) {
                ssize_t w = write(out_fd, buf + off, (size_t)(n - off));
                if (w < 0) {
                    if (errno == EINTR) continue;
                    perror("write out");
                    close(out_fd);
                    close(to_upper[0]);
                    _exit(EXIT_FAILURE);
                }
                off += w;
            }
        }

        close(out_fd);
        close_checked(to_upper[0]);
        _exit(EXIT_SUCCESS);
    }

    // ---- Padre: lee fichero y escribe a ambos pipes ----
    close_checked(to_grep[0]);   // padre escribe
    close_checked(to_upper[0]);  // padre escribe

    unsigned char buf[4096];
    for (;;) {
        ssize_t n = read(in_fd, buf, sizeof(buf));
        if (n == 0) break;
        if (n < 0) {
            if (errno == EINTR) continue;
            die("read input");
        }

        // Escribir al pipe de grep
        ssize_t off = 0;
        while (off < n) {
            ssize_t w = write(to_grep[1], buf + off, (size_t)(n - off));
            if (w < 0) {
                if (errno == EINTR) continue;
                die("write to_grep");
            }
            off += w;
        }

        // Escribir al pipe de upper
        off = 0;
        while (off < n) {
            ssize_t w = write(to_upper[1], buf + off, (size_t)(n - off));
            if (w < 0) {
                if (errno == EINTR) continue;
                die("write to_upper");
            }
            off += w;
        }
    }

    close_checked(in_fd);
    close_checked(to_grep[1]);
    close_checked(to_upper[1]);

    int status;
    if (waitpid(grep_pid, &status, 0) == -1) die("waitpid grep");
    if (waitpid(upper_pid, &status, 0) == -1) die("waitpid upper");

    return EXIT_SUCCESS;
}
