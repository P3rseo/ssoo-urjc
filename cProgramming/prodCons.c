#include <stdio.h>
#include <pthread.h>

#define TOTAL_DATOS 10000
#define BUFFER_SIZE 1024

int n_datos = 0; // Cuando hay dato se pone a 1
int dato = 0;

// Variables
pthread_mutex_t mutex;                  // Mutex (lock y unlock)
pthread_cond_t hay_hueco, hay_dato;     // Condiciones (hay dato o hay hueco)

// Funciones
void *Productor();     // Funcion Productor
void *Consumidor();    // Funcion Consumidor

int main() {

    pthread_t th1, th2; // Inicializar los threads.

    if (pthread_mutex_init(&mutex, NULL) != 0) { /* Inicializar el mutex */
        perror("pthread_mutex_init");
        return 1;
    } 
    if (pthread_cond_init(&hay_hueco, NULL) != 0) { /* Inicializar condicion hay_hueco */
        perror("pthread_cond_init(hay_hueco)");
        return 1;
    }    
    if (pthread_cond_init(&hay_dato, NULL) != 0) { /* Inicializar condicion hay_dato */
        perror("pthread_cond_init(hay_dato)");
        return 1;
    }     
    
    if (pthread_create(&th2, NULL, Consumidor, NULL) != 0) { /* Creamos el thread del Consumidor */
        perror("pthread_create(Consumidor)");
        return 1;
    }
    if (pthread_create(&th1, NULL, Productor, NULL) != 0) { /* Creamos el thread del Productor */
        perror("pthread_create(Productor)");
        return 1;
    }


    if(pthread_join(th1, NULL) != 0) { /* Sincronizar de forma blocante */
        perror("pthread_join(th1)");
        return 1;
    }
    if(pthread_join(th2, NULL) != 0) { /* Sincronizar de forma blocante */
        perror("pthread_join(th2)");
        return 1;
    }

    if (pthread_mutex_destroy(&mutex) != 0) { /* Destruir mutex */
        perror("pthread_mutex_destroy");
        return 1;
    }
    if (pthread_cond_destroy(&hay_hueco) != 0) { /* Destruir condicion hay_hueco */
        perror("pthread_cond_destroy(hay_hueco)");
        return 1;
    }
    if (pthread_cond_destroy(&hay_dato) != 0) { /* Destruir condicion hay_dato */
        perror("pthread_cond_destroy(hay_dato)");
        return 1;
    }


    return 0;
}

void *Productor() {
    //(void *)arg;
    printf("<Productor> Hola, Soy el Productor\n");

    pthread_mutex_lock(&mutex);
    printf("<Productor> Soy el Productor desde su seccion critica\n");
    while(n_datos != 0) {
        printf("<Productor> Necesito que me quiten el dato primero, cedo a Consumidor\n");
        pthread_cond_wait(&hay_hueco, &mutex);
    }

    printf("<Productor> Se ha escrito un dato\n");
    dato++;
    n_datos++;
    pthread_cond_signal(&hay_dato);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *Consumidor() {
    //(void *)arg;
    printf("<Consumidor> Hola, Soy el Consumidor\n");

    pthread_mutex_lock(&mutex);
    printf("<Consumidor> Soy el Consumidor desde su seccion critica\n");
    while(n_datos == 0) {
        printf("<Consumidor> Necesito que haya un dato primero, cedo a Productor\n");
        pthread_cond_wait(&hay_dato, &mutex);
    }

    printf("<Consumidor> El dato recibido es: %d\n", dato);
    n_datos--;
    pthread_cond_signal(&hay_hueco);
    pthread_mutex_unlock(&mutex);

    return NULL;
}