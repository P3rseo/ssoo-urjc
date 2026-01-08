#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define TRABAJADORES 10 // 20 en realidad
#define NUM_PIEZAS 30 // 10000 en realidad
#define SOLDAR 3
#define CARRACA 4
#define LASER 1

static int piezas = 0;
static int restantes = NUM_PIEZAS;

// MUTEX CONTADOR
pthread_mutex_t m_contador;

// 1. SOLDAR
int soldar = SOLDAR;
pthread_mutex_t m_soldar;
pthread_cond_t c_soldar;

// 2. CARRACA
int carraca = CARRACA;
pthread_mutex_t m_carraca;
pthread_cond_t c_carraca;

// 3. LASER
int laser = LASER;
pthread_mutex_t m_laser;
pthread_cond_t c_laser;

// Funciones
void *trabajador(void *num);

int main() {

    pthread_t thread[TRABAJADORES];
    int id_trabajadores[TRABAJADORES];

    // Inicializar mutex
    if (pthread_mutex_init(&m_contador, NULL) != 0) {
        perror("pthread_mutex_init contador");
        return 1;
    }
    if (pthread_mutex_init(&m_soldar, NULL) != 0) {
        perror("pthread_mutex_init soldar");
        return 1;
    }
    if (pthread_mutex_init(&m_carraca, NULL) != 0) {
        perror("pthread_mutex_init carraca");
        return 1;
    }
    if (pthread_mutex_init(&m_laser, NULL) != 0) {
        perror("pthread_mutex_init laser");
        return 1;
    }

    // Inicializar condiciones
    if (pthread_cond_init(&c_soldar, NULL) != 0) {
        perror("pthread_cond_init soldar");
        return 1;
    }
    if (pthread_cond_init(&c_carraca, NULL) != 0) {
        perror("pthread_cond_init carraca");
        return 1;
    }
    if (pthread_cond_init(&c_laser, NULL) != 0) {
        perror("pthread_cond_init laser");
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

    // DESTRUIR CONDICIONES
    pthread_cond_destroy(&c_soldar);
    pthread_cond_destroy(&c_carraca);
    pthread_cond_destroy(&c_laser);

    // DESTRUIR MUTEX
    pthread_mutex_destroy(&m_contador);
    pthread_mutex_destroy(&m_soldar);
    pthread_mutex_destroy(&m_carraca);
    pthread_mutex_destroy(&m_laser);

    return 0;
}

void *trabajador(void *num) {
    int id = *(int *) num;

    printf("Trabajador %d entra a trabajar\n", id);

    while (1) {
        // Miramos si ya estan las piezas al completo.
        pthread_mutex_lock(&m_contador);
        if(restantes == 0) {
            pthread_mutex_unlock(&m_contador);
            printf("Trabajador %d termina su jornada laboraaal !!\n", id);
            return NULL;
        }
        restantes--;
        pthread_mutex_unlock(&m_contador);

        // 1. Soldar
        printf("Trabajador %d quiere soldar\n", id);

        pthread_mutex_lock(&m_soldar);
        while(soldar == 0) {
            pthread_cond_wait(&c_soldar, &m_soldar);
        }
        soldar--;
        pthread_mutex_unlock(&m_soldar);

        printf("Trabajador %d soldando\n", id);
        sleep(1 + (rand() % 5));

        pthread_mutex_lock(&m_soldar);
        soldar++;
        pthread_cond_signal(&c_soldar);
        pthread_mutex_unlock(&m_soldar);

        // 2. Carraca
        printf("Trabajador %d quiere usar llave de carraca\n", id);

        pthread_mutex_lock(&m_carraca);
        while(carraca == 0) {
            pthread_cond_wait(&c_carraca, &m_carraca);
        }
        carraca--;
        pthread_mutex_unlock(&m_carraca);

        printf("Trabajador %d usando carraca\n", id);
        sleep(1 + (rand() % 5));
        
        pthread_mutex_lock(&m_carraca);
        carraca++;
        pthread_cond_signal(&c_carraca);
        pthread_mutex_unlock(&m_carraca);

        // 3. Laser
        printf("Trabajador %d quiere usar laser\n", id);

        pthread_mutex_lock(&m_laser);
        while(laser == 0) {
            pthread_cond_wait(&c_laser, &m_laser);
        }
        laser--;
        pthread_mutex_unlock(&m_laser);

        printf("Trabajador %d usando laser\n", id);
        sleep(1 + (rand() % 5));
        
        pthread_mutex_lock(&m_laser);
        laser++;
        pthread_cond_signal(&c_laser);
        pthread_mutex_unlock(&m_laser);

        // Aumentar el numero de piezas
        pthread_mutex_lock(&m_contador);
        piezas++;
        printf("Trabajador %d. Contador piezas: %d\n", id, piezas);
        pthread_mutex_unlock(&m_contador);

    }

    return NULL;
}