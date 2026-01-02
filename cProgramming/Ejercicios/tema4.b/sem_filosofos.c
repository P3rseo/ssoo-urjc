#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define FILOSOFOS 6

// variables
sem_t palillo[FILOSOFOS];

// funciones
void* filosofo(void* num);

int main(void) {
    pthread_t th[FILOSOFOS];
    int fil_id[FILOSOFOS];

    for (int i = 0; i < FILOSOFOS; i++) {
        if(sem_init(&palillo[i], 0, 1) != 0) {
            perror("sem_init");
            return 1;
        }
    }

    for (int i = 0; i < FILOSOFOS; i++) {
        fil_id[i] = i;
        if (pthread_create(&th[i], NULL, filosofo, (void*)&fil_id[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    while(1) pause();

    return 0;
}


void *filosofo(void* num) {
    int id = *(int *) num;

    while (1) {
        printf("Filosofo %d pensando\n", id);

        sleep((rand() % 5) + 1); // Entre 1-5 segundos

        printf("Filosofo %d quiere comer\n", id);
        
        // ENTRANDO EN SECCION CRITICA
        // filosofos pares cogen primero palillo izquierdo e impares derecho. 
        if (id % 2 == 0) {
            sem_wait(&palillo[id % FILOSOFOS]);
            sem_wait(&palillo[(id + 1) % FILOSOFOS]);
        }
        else {
            sem_wait(&palillo[(id + 1) % FILOSOFOS]);
            sem_wait(&palillo[id % FILOSOFOS]);
        }

        printf("Filosofo %d comiendo\n", id);

        sleep((rand() % 5) + 1); // Entre 1-5 segundos

        if (id % 2 != 0) {
            sem_post(&palillo[id % FILOSOFOS]);
            sem_post(&palillo[(id + 1) % FILOSOFOS]);
        }
        else {
            sem_post(&palillo[(id + 1) % FILOSOFOS]);
            sem_post(&palillo[id % FILOSOFOS]);
        }
        // SALIENDO DE SECCION CRITICA
    }

    return NULL;
}