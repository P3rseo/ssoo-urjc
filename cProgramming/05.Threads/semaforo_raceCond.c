#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// Variable compartida
volatile int j = 0;
sem_t sem;


void *th_func1() {
    int i;
    for (i = 0; i < 1000000; i++) {
        sem_wait(&sem); // Entrada seccion critica
        j += 1;
        sem_post(&sem); // Salir seccion critica
    }

    pthread_exit(NULL);
}

int main () {
    pthread_t th1, th2;

    // sem_init(&sem, pshared, value)
    // pshared=0: semaforo compartido entre hilos del mismo proceso
    if (sem_init(&sem, 0, 1) != 0) {
        perror("sem_init");
        return 1;
    }

    pthread_create(&th1, NULL, th_func1, NULL);
    pthread_create(&th2, NULL, th_func1, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    sem_destroy(&sem);

    printf("j = %d\n", j); // 20000? -- NO, por que al tratarse de un recurso compartido ambos estan accediendo y modificando la variable, alterando su estado.

}