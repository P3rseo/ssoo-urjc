#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // hijo
        sleep(1);
        printf("[HIJO]  getpid()=%d, getppid()=%d, fork() devolvio pid=%d\n",
               getpid(), getppid(), pid);
        exit(7);
    }

    // padre
    printf("[PADRE] getpid()=%d, fork() devolvio pid=%d (PID del hijo)\n",
           getpid(), pid);

    int status;
    pid_t ended = wait(&status);

    printf("[PADRE] wait() devolvio ended=%d\n", ended);

    if (WIFEXITED(status)) {
        printf("[PADRE] El hijo terminó con exit code=%d\n", WEXITSTATUS(status));
    }

    return 0;
}

