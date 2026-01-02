#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

static int buffer;          // "buffer de 1 posicion"
static sem_t empty, full;   // sincronizacion   <1> , <0>
static sem_t mutex;         // exclusion mutua  <1>

void *productor(/*void *arg*/) {
    for (int i = 1; i <= 10; i++) {

        sem_wait(&empty);   // espera a que haya hueco (buffer vacio)
        sem_wait(&mutex);   // entra en 

        buffer = i;
        printf("Produzco %d\n", buffer);

        sem_post(&mutex);
        sem_post(&full);

        usleep(100000);
    }

    pthread_exit(NULL);
}

void *consumidor(/*void *arg*/) {
    for (int i = 1; i <= 10; i++) {

        sem_wait(&full);    // espera a que haya dato (buffer lleno)
        sem_wait(&mutex);   // entra en seccion critica

        int x = buffer;
        printf("Consumo %d\n", x);

        sem_post(&mutex);   // sale de seccion critica 
        sem_post(&empty);   // avisa: ahora hay hueco (buffer vacio)

        usleep(150000);
    }

    pthread_exit(NULL);
}

int main(void) {
    pthread_t tp, tc;

    // Inicializacion semaforos
    // empry=1 (hay hueco), full=0 (no hay dato), mutex=1 (libre)
    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    
    pthread_create(&tp, NULL, productor, NULL);
    pthread_create(&tc, NULL, consumidor, NULL);

    pthread_join(tp, NULL);
    pthread_join(tc, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    
    return 0;
}