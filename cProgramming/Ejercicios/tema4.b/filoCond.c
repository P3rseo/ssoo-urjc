#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define FILOSOFOS 5                 // Numero de filosofos con el que estamos trabajando.

int palillo[FILOSOFOS];             // Esto nos indica cuando los palillos estan ocupados (1) o libres (0). Empiezan en 0.

pthread_mutex_t mutex;              // Solo tenemos un mutex por que solo hay una seccion critica que proteger, que son los palillos.
pthread_cond_t espera[FILOSOFOS];   // Tenemos una espera para cada uno de los filosofos, que les indica cuando tienen los palillos libres.

void *Filosofo(void* num);          // Funcion Filosofo

int main() {
    pthread_t threads[FILOSOFOS];   // Hacemos tantos threads como filosofos haya
    int fil_id[FILOSOFOS];          // Con esto recogemos el id de los diferentes filosofos

    if (pthread_mutex_init(&mutex, NULL) != 0) { /* Inicializar el mutex */
        perror("pthread_mutex_init");
        return 1;
    }
    
    for (int i = 0; i < FILOSOFOS; i++) { 
        fil_id[i] = i;      // id de cada filosofo
        palillo[i] = 0;     // Inicializamos los palillos a 0

        if (pthread_cond_init(&espera[i], NULL) != 0) { /* Inicializamos las condiciones de cada filosofo */
        perror("pthread_cond_init(libre)");
        return 1;
        }

        if (pthread_create(&threads[i], NULL, Filosofo, (void*)&fil_id[i]) != 0) { /* Creamos los threads con la funcion filosofo y su id */
            perror("pthread_create");
            return 1;
        }
    }

    while(1) pause();   // Hacemos un bucle infinito

    return 0;

}

void *Filosofo(void *num) {
    
    int id = *(int *) num;  // id del filosofo

    while (1) { /* Bucle infinito (se ejecutara todo el rato) */

        printf("Filosofo %d pensando\n", id);

        sleep(1 + (rand() % 5));

        printf("Filosofo %d quiere comer\n", id);

        int izq = id;                       // izquierda es el palillo con el mismo numero (id) que el filosofo
        int der = (id + 1) % FILOSOFOS;     // derecha es el palillo de la derecha del filosofo. Necesitamos el modulo para hacerlo circular
        
        pthread_mutex_lock(&mutex);                     // Cogemos el mutex para mirar si estan libres los palillos
        while(palillo[izq] || palillo[der]) {           // Mientras que los palillos esten ocupados, espera la condicion
            pthread_cond_wait(&espera[id], &mutex);     // Filosofo espera a que le liberen sus palillos y le envien una senial. 
        }                                               // F0 (p0, p1) -> F1 (p1) y F4 (p0)
        
        // Entramos en la seccion critica
        palillo[izq] = 1;   // Como estamos usando el palillo, lo ponemos a 1
        palillo[der] = 1;   // Como estamos usando el palillo, lo ponemos a 1

        pthread_mutex_unlock(&mutex);   // Fin mutex

        printf("Filosofo %d comiendo\n", id);

        sleep(1 + (rand() % 5));

        pthread_mutex_lock(&mutex); // Entrando seccion critica
        palillo[izq] = 0;   // Dejamos de usar el palillo, lo ponemos a 0
        palillo[der] = 0;   // Dejamos de usar el palillo, lo ponemos a 0

        // Como hemos dejado de usar los palillos:
        pthread_cond_signal(&espera[(id + FILOSOFOS - 1) % FILOSOFOS]); // Avisamos al filosofo izquierdo que el palillo esta libre
        pthread_cond_signal(&espera[(id + 1) % FILOSOFOS]);             // Avisamos al filosofo derecho que el palillo esta libre
        pthread_mutex_unlock(&mutex); // Saliendo seccion critica
    }
    return NULL;
}