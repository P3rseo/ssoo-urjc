#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_PLAZAS 5
#define NUM_COCHES 12

static sem_t plazas;                 // contador de plazas disponibles
static pthread_mutex_t mutex;
static int libres = NUM_PLAZAS;      // solo para mostrar en pantalla (protegido con mutex)

static void *coche(void *arg) {
    long id = (long)arg;

    // Simula llegadas en momentos distintos
    usleep((rand() % 400) * 1000);

    printf("Coche %ld: quiere entrar\n", id);

    // Espera a que haya una plaza libre
    sem_wait(&plazas);

    // Ya tiene plaza: actualizamos "libres" de forma segura
    pthread_mutex_lock(&mutex);
    libres--;
    printf("Coche %ld: ENTRA. Plazas libres = %d\n", id, libres);
    pthread_mutex_unlock(&mutex);

    // Simula tiempo aparcado
    usleep((300 + rand() % 700) * 1000);

    // Sale: actualiza contador y libera plaza
    pthread_mutex_lock(&mutex);
    libres++;
    printf("Coche %ld: SALE.  Plazas libres = %d\n", id, libres);
    pthread_mutex_unlock(&mutex);

    sem_post(&plazas);
    return NULL;
}

int main(void) {
    srand((unsigned)time(NULL));

    if (sem_init(&plazas, 0, NUM_PLAZAS) != 0) {
        perror("sem_init");
        return 1;
    }

    pthread_t hilos[NUM_COCHES];
    pthread_mutex_init(&mutex, NULL);

    for (long i = 0; i < NUM_COCHES; i++) {
        if (pthread_create(&hilos[i], NULL, coche, (void *)i) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < NUM_COCHES; i++) {
        pthread_join(hilos[i], NULL);
    }

    sem_destroy(&plazas);
    return 0;
}
