#include <stdio.h>
#include <pthread.h>

int sum = 0;

void *suma(void *num) { /* hebra */
    int limite = *(int *) num;

    for (int i = 0; i <= limite; i++) {
        sum += i;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    int N = 10;

    pthread_create(&tid, NULL, suma, (void *) &N); // <Thread ID> | <Thread Attributes> | <Function Thread will execute> | <SIngle Argument passed to Function>
    pthread_join(tid, NULL);
    printf("SUMA = %d\n", sum); // SUMA = suma de los N primeros numeros
    return 0;
}