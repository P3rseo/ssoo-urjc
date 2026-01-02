#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Variable compartida
volatile int j = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;


void *th_func1() {
    int i;
    for (i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&m);
        j += 1;
        pthread_mutex_unlock(&m);
    }

    pthread_exit(NULL);
}

int main () {
    pthread_t th1, th2;

    pthread_create(&th1, NULL, th_func1, NULL);
    pthread_create(&th2, NULL, th_func1, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("j = %d\n", j); // 20000? -- NO, por que al tratarse de un recurso compartido ambos estan accediendo y modificando la variable, alterando su estado.

}