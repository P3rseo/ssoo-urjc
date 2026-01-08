#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRABAJADORES 10 // 20 en realidad
#define NUM_PIEZAS 30 // 10000 en realidad
#define SOLDAR 3
#define CARRACA 4
#define LASER 1

static int piezas = 0;
static int restantes = NUM_PIEZAS;

sem_t s_contador;
sem_t s_soldar;     // (3)
sem_t s_carraca;    // (4)
sem_t s_laser;      // (1)

// Funciones
void *trabajador(void *num);

int main() {

    pthread_t thread[TRABAJADORES];
    int id_trabajadores[TRABAJADORES];

    // Inicializar semaforos
    if (sem_init(&s_contador, 0, 1) != 0) {
        perror("sem_init contador");
        return 1;
    }
    if (sem_init(&s_soldar, 0, SOLDAR) != 0) {
        perror("sem_init soldar");
        return 1;
    }
    if (sem_init(&s_carraca, 0, CARRACA) != 0) {
        perror("sem_init carraca");
        return 1;
    }
    if (sem_init(&s_laser, 0, LASER) != 0) {
        perror("sem_init laser");
        return 1;
    }

    // Crear los threads
    for (int i = 0; i < TRABAJADORES; i++) {

        id_trabajadores[i] = i;

        if (pthread_create(&thread[i], NULL, trabajador, (void*)&id_trabajadores[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < TRABAJADORES; i++) {
        if (pthread_join(thread[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    printf("Se han construido las piezas necesarias para el caza TIE\n");

    sem_destroy(&s_contador);
    sem_destroy(&s_soldar);
    sem_destroy(&s_carraca);
    sem_destroy(&s_laser);

    return 0;
}

void *trabajador(void *num) {
    int id = *(int *) num;

    printf("Trabajador %d entra a trabajar\n", id);

    while (1) {
        // Miramos si ya estan las piezas al completo.
        sem_wait(&s_contador);
        if(restantes == 0) {
            sem_post(&s_contador);
            printf("Trabajador %d termina su jornada laboraaal !!\n", id);
            return NULL;
        }
        restantes--;
        sem_post(&s_contador);

        // 1. Soldar
        printf("Trabajador %d quiere soldar\n", id);

        sem_wait(&s_soldar);
        printf("Trabajador %d soldando\n", id);
        sleep(1 + (rand() % 5));
        sem_post(&s_soldar);

        // 2. Carraca
        printf("Trabajador %d quiere usar llave de carraca\n", id);

        sem_wait(&s_carraca);
        printf("Trabajador %d usando carraca\n", id);
        sleep(1 + (rand() % 5));
        sem_post(&s_carraca);

        // 3. Laser
        printf("Trabajador %d quiere usar laser\n", id);

        sem_wait(&s_laser);
        printf("Trabajador %d usando laser\n", id);
        sleep(1 + (rand() % 5));
        sem_post(&s_laser);

        // Aumentar el numero de piezas
        sem_wait(&s_contador);
        piezas++;
        printf("Trabajador %d. Contador piezas: %d\n", id, piezas);
        sem_post(&s_contador);
    }

    return NULL;
}