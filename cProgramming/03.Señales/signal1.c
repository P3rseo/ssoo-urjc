# include <stdio.h>
# include <signal.h>
# include <unistd.h>

void manejador (int sig)
{
    printf("\nRecibida senial %d\n", sig);
}

int main()
{
    signal(SIGINT, manejador);
    printf("Esperando senial.\n");
    pause();
    printf("Saliendo.\n");
    return 0;
}