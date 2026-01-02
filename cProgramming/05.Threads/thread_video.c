#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// CUANDO MUERE EL PROCESO MUEREN TODOS LOS THREADS.

// prototipo
void *work(void *arg);

int main () {

    pthread_t tid; // Thread ID
    long valor1 = 1;

    long *res;
    
    if (pthread_create(&tid, NULL, work, (void *)&valor1) != 0) {
        return 1;
    }
    
    if (pthread_join(tid, (void**)&res) != 0) { /* Con esto espera a que tid termine. */
        return 1;
    }
    
    printf("res = %ld\n", *res);

    free(res);

    return 0;
}


void *work(void *arg) {
    
    long *valor = (long *)(arg);
    long suma = 0, i;
    long *r = (long *)malloc(sizeof(long));

    for (i = 0; i < 10000000; i++) {
        suma += *valor;
    }

    printf("valor = %ld\n", *valor);

    *r = suma;

    return (void *)r; // Esto no puede ser por que solo existe en el ambito de la funcion work. Se crea en el stack.
}