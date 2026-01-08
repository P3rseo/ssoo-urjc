#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define ELFOS 6
#define NUM_JUGUETES 20
#define NUM_CAJAS 3
#define NUM_MESAS 2
#define NUM_ESCALERAS 1

void *elfo(void *num);

static int contador_juguetes = 0;
static int restantes = NUM_JUGUETES;

sem_t cajaHerramientas; // Contador de cajas disponibles
sem_t mesas;            // Contador mesas disponibles
sem_t escalera;         // Contador escalera disponible
sem_t sem;              // Modificar el contador de juguetes

int main() {
    pthread_t thread[ELFOS];
    int elfo_id[ELFOS];

    // Inicializar semafotos
    if (sem_init(&cajaHerramientas, 0, NUM_CAJAS) != 0) {
        perror("sem_init cajas");
        return 1;
    }
    if (sem_init(&mesas, 0, NUM_MESAS) != 0) {
        perror("sem_init mesas");
        return 1;
    }
    if (sem_init(&escalera, 0, NUM_ESCALERAS) != 0) {
        perror("sem_init escalera");
        return 1;
    }
    if (sem_init(&sem, 0, 1) != 0) {
        perror("sem_init sem");
        return 1;
    }

    // Crear threads
    for (int i = 0; i < ELFOS; i++) {
        elfo_id[i] = i; 
        if (pthread_create(&thread[i], NULL, elfo, (void*)&elfo_id[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // Esperar a que terminen todos
    for (int i = 0; i < ELFOS; i++) {
        pthread_join(thread[i], NULL);
    }

    printf("\nFIN: Se han fabricado, envuelto y almacenado %d regalos.\n", contador_juguetes);

    sem_destroy(&cajaHerramientas);
    sem_destroy(&mesas);
    sem_destroy(&escalera);
    sem_destroy(&sem);

    return 0;
}

void *elfo(void *num) {

    int id = *(int *) num;

    printf("Elfo %d entrando al taller\n", id);

    while (1) {

        sem_wait(&sem);
        if(restantes == 0) {
            sem_post(&sem);
            return NULL;
        }
        restantes--;
        sem_post(&sem);

        // Fabricar
        printf("Elfo %d quiere fabricar un juguete\n", id);
        
        sem_wait(&cajaHerramientas);
        printf("Elfo %d fabricando un juguete\n", id);
        sleep(1 + (rand() % 5));
        sem_post(&cajaHerramientas);

        // Envolver
        printf("Elfo %d quiere envolver un juguete\n", id);

        sem_wait(&mesas);
        printf("Elfo %d envolviendo juguete\n", id);
        sleep(1 + (rand() % 5));
        sem_post(&mesas);

        // Almacenar
        printf("Elfo %d quiere colocar un juguete\n", id);

        sem_wait(&escalera);
        printf("Elfo %d colocando juguete\n", id);
        sleep(1 + (rand() % 5));
        sem_post(&escalera);

        sem_wait(&sem);
        contador_juguetes++;
        printf("Elfo %d. Contador de juguetes: %d\n", id, contador_juguetes);
        sem_post(&sem);

        printf("Elfo %d descansando\n", id);
        sleep(1 + (rand() % 5));
    }

    return NULL;
}