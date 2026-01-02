#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define FILOSOFOS 5

// variables
pthread_mutex_t palillo[FILOSOFOS];

// funciones
void* filosofo(void* num);

int main(void) {
    pthread_t th[FILOSOFOS];
    int fil_id[FILOSOFOS];

    for (int i = 0; i < FILOSOFOS; i++) {
        if(pthread_mutex_init(&palillo[i], NULL) != 0) {
            perror("pthread_mutex_init");
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

        int izq = id;
        int der = (id + 1) % FILOSOFOS; 
        // ENTRANDO EN SECCION CRITICA
        // filosofos pares cogen primero palillo izquierdo e impares derecho. 
        if (id % 2 == 0) {
            pthread_mutex_lock(&palillo[izq]);
            pthread_mutex_lock(&palillo[der]);
        }
        else {
            pthread_mutex_lock(&palillo[der]);
            pthread_mutex_lock(&palillo[izq]);
        }

        printf("Filosofo %d comiendo\n", id);

        sleep((rand() % 5) + 1); // Entre 1-5 segundos

        // No se necesita un orden para soltar
        pthread_mutex_unlock(&palillo[izq]);
        pthread_mutex_unlock(&palillo[der]);
        // SALIENDO DE SECCION CRITICA
    }

    return NULL;
}