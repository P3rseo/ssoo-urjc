#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ELFOS 6
#define NUM_JUGUETES 20
#define NUM_CAJAS 3
#define NUM_MESAS 2
#define NUM_ESCALERAS 1

void *elfo(void *num);

static int contador_juguetes = 0;
static int restantes = NUM_JUGUETES;

pthread_mutex_t m_contador; // Modificar el contador de juguetes

// Recurso: caja herramientas (3)
static int cajas_libres = NUM_CAJAS;
pthread_mutex_t m_cajas;
pthread_cond_t c_cajas;

// Recurso: mesas (2)
static int mesas_libres = NUM_MESAS;
pthread_mutex_t m_mesas;
pthread_cond_t c_mesas;

// Recurso: escalera (1)
static int escaleras_libres = NUM_ESCALERAS;
pthread_mutex_t m_escaleras;
pthread_cond_t c_escaleras;


int main() {
    pthread_t thread[ELFOS];
    int elfo_id[ELFOS];

    // Inicializar mutex
    if(pthread_mutex_init(&m_contador, NULL) != 0) {
        perror("pthread_mutex_init contador");
        return 1;
    }
    if(pthread_mutex_init(&m_cajas, NULL) != 0) {
        perror("pthread_mutex_init cajas");
        return 1;
    }
    if(pthread_mutex_init(&m_mesas, NULL) != 0) {
        perror("pthread_mutex_init mesas");
        return 1;
    }
    if(pthread_mutex_init(&m_escaleras, NULL) != 0) {
        perror("pthread_mutex_init escaleras");
        return 1;
    }

    // Inicializar condiciones
    if(pthread_cond_init(&c_cajas, NULL) != 0) {
        perror("pthread_cond_init cajas");
        return 1;
    }
    if(pthread_cond_init(&c_mesas, NULL) != 0) {
        perror("pthread_cond_init mesas");
        return 1;
    }
    if(pthread_cond_init(&c_escaleras, NULL) != 0) {
        perror("pthread_cond_init escaleras");
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

    // Destruir condiciones
    pthread_cond_destroy(&c_cajas);
    pthread_cond_destroy(&c_mesas);
    pthread_cond_destroy(&c_escaleras);

    // Destruir mutex
    pthread_mutex_destroy(&m_contador);
    pthread_mutex_destroy(&m_cajas);
    pthread_mutex_destroy(&m_mesas);
    pthread_mutex_destroy(&m_escaleras);


    return 0;
}

void *elfo(void *num) {

    int id = *(int *) num;

    printf("Elfo %d entrando al taller\n", id);

    while (1) {

        pthread_mutex_lock(&m_contador);            // Bloqueo el mutex contador
        if(restantes == 0) {                        // Si no quedan juguetes por hacer:
            pthread_mutex_unlock(&m_contador);          // Desbloqueo mutex contador
            printf("Elfo %d termina\n", id);
            return NULL;                                // Devuelvo NULL
        }                                           // Si quedan juguetes por hacer:
        restantes--;                                    // Restamos un juguete
        pthread_mutex_unlock(&m_contador);              // Desbloqueo mutex contador

        // Fabricar
        printf("Elfo %d quiere fabricar un juguete\n", id);
        
        pthread_mutex_lock(&m_cajas);                   // Bloqueo mutex de cajas
        while(cajas_libres == 0) {                      // Mientras no hay cajas libres:
            pthread_cond_wait(&c_cajas, &m_cajas);          // Esperamos a que haya cajas libres
        }
        cajas_libres--;                                 // Quitamos caja que usamos
        pthread_mutex_unlock(&m_cajas);                 // Desbloqueo mutex de cajas

        printf("Elfo %d fabricando un juguete\n", id);
        sleep(1 + (rand() % 5));
        
        pthread_mutex_lock(&m_cajas);       // Bloqueo mutex de cajas
        cajas_libres++;                     // Sumo la caja que dejo de usar
        pthread_cond_signal(&c_cajas);      // Mando senial de que hay caja
        pthread_mutex_unlock(&m_cajas);     // Desbloqueo mutex de cajas

        // Envolver
        printf("Elfo %d quiere envolver un juguete\n", id);

        pthread_mutex_lock(&m_mesas);                   // Bloqueo mutex de mesas
        while(mesas_libres == 0) {                      // Si no hay mesas libres
            pthread_cond_wait(&c_mesas, &m_mesas);          // Espero una mesa libre
        }               
        mesas_libres--;                                 // Resto mesa que voy a usar
        pthread_mutex_unlock(&m_mesas);                 // Desbloqueo mutex de mesas

        printf("Elfo %d envolviendo juguete\n", id);
        sleep(1 + (rand() % 5));

        pthread_mutex_lock(&m_mesas);       // Bloqueo mutex de mesas
        mesas_libres++;                     // Sumo mesa que dejo de usar
        pthread_cond_signal(&c_mesas);      // Mando senial de que hay mesa libre
        pthread_mutex_unlock(&m_mesas);     // Desbloqueo mutex de mesas

        // Almacenar
        printf("Elfo %d quiere colocar un juguete\n", id);

        pthread_mutex_lock(&m_escaleras);
        while(escaleras_libres == 0) {
            pthread_cond_wait(&c_escaleras, &m_escaleras);
        }
        escaleras_libres--;
        pthread_mutex_unlock(&m_escaleras);

        printf("Elfo %d colocando juguete\n", id);
        sleep(1 + (rand() % 5));

        pthread_mutex_lock(&m_escaleras);
        escaleras_libres++;
        pthread_cond_signal(&c_escaleras);
        pthread_mutex_unlock(&m_escaleras);

        pthread_mutex_lock(&m_contador);
        contador_juguetes++;
        printf("Elfo %d. Contador de juguetes: %d\n", id, contador_juguetes);
        pthread_mutex_unlock(&m_contador);

        printf("Elfo %d descansando\n", id);
        sleep(1 + (rand() % 5));
    }

    printf("Elfo %d termina", id);

    return NULL;
}