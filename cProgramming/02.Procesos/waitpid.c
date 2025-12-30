#include <stdio.h>
#include <unistd.h>     // fork, pid_t, 
#include <stdlib.h>     // exit
#include <sys/wait.h>   // waitpid

int main (void) {
    pid_t a = fork();
    if (a == 0) { sleep(2); exit(1); }

    pid_t b = fork();
    if (b == 0) { sleep(1); exit(2); }

    int status;
    pid_t ended;

    // Espero SOLO al hijo b (aunque a se cree antes).
    ended = waitpid(b, &status, 0);

    if (WIFEXITED(status)) {
        printf("Espere al hijo b con PID %d y salio con codigo %d\n", ended, WEXITSTATUS(status));
    }

    ended = waitpid(-1, &status, 0);
    if (WIFEXITED(status)) {
        printf("Espere al hijo a con PID %d y salio con codigo %d\n", ended, WEXITSTATUS(status));
    }
    return 0;
}