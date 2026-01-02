#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define FILOSOFOS 5

// variables
int palillo[FILOSOFOS];
int estado_filosofo[FILOSOFOS];
pthread_mutex_t mutex;
pthread_cond_t espera[FILOSOFOS];

// funciones
void* filosofo(void* num);

int main(void) {
    pthread_t th[FILOSOFOS];
    int fil_id[FILOSOFOS];

    for (int i = 0; i < FILOSOFOS; i++) {
        if(pthread_mutex_init(&espera[i], NULL) != 0) {
            perror("pthread_mutex_init");
            return 1;
        }
    }

    for (int i = 0; i < FILOSOFOS; i++) {
        fil_id[i] = i;
        palillo[i] = 0;

        if (pthread_cond_init(&espera[i], NULL) != 0) {
            perror("pthread_cond_init");
            return 1;
        }

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
        estado_filosofo[id] = 0;
        printf("Filosofo %d pensando\n", id);
        print_estado();

        sleep((rand() % 5) + 1); // Entre 1-5 segundos

        estado_filosofo[id] = 1;
        printf("Filosofo %d quiere comer\n", id);
        print_estado();

        int izq = id;
        int der = (id + 1) % FILOSOFOS; 
        
        pthread_mutex_lock(&mutex);
        while((palillo[izq]) || (palillo[der])) {
            pthread_cond_wait(&espera[id], &mutex);
        }
        // ENTRANDO EN SECCION CRITICA
        palillo[izq] = 1;
        palillo[der] = 1;

        pthread_mutex_unlock(&mutex);

        estado_filosofo[fil_id] = 2;
        printf("Filosofo %d comiendo\n", id);
        print_estado();

        sleep((rand() % 5) + 1); // Entre 1-5 segundos

        pthread_mutex_lock(&mutex);
        palillo[izq] = 0;
        palillo[der] = 0;
        
        pthread_mutex_unlock(&palillo[izq]);
        pthread_mutex_unlock(&palillo[der]);
        pthread_cond_signal(&espera[izq]);	
		pthread_cond_signal(&espera[der]);
		pthread_mutex_unlock(&mutex); // Saliendo de sección crítica
        // SALIENDO DE SECCION CRITICA
    }

    return NULL;
}