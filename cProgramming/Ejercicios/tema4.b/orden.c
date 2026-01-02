#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void *saludo1(void *arg);
void *saludo2(void *arg);

int main() {

    // Inicializar 2 threads
    pthread_t th1, th2;

    // Inicializar semaforo
    if (sem_init(&sem, 0, 0) != 0) {
        perror("sem_init");
        return 1;
    }

    // Crear threads
    if (pthread_create(&th1, NULL, saludo1, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }
    if (pthread_create(&th2, NULL, saludo2, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Conectar threads
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    // Cerrar semaforo
    sem_destroy(&sem);

    return 0;
}

void *saludo1(void *arg) {
    (void) arg;

    sem_wait(&sem);
    printf("Saludos desde el thread 1\n");
    return NULL;
}

void *saludo2(void *arg) {
    (void) arg;

    printf("Saludos desde el thread 2\n");
    sem_post(&sem);
    return NULL;
}

