#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define CANTIDAD_A_RETIRAR 1000
#define NOMINA 3000

volatile int saldo = 0;

pthread_mutex_t mutex_saldo;            // Aceso a la seccion critica
pthread_cond_t cond_saldo_mayor1000;    // Condicion: mientras saldo < 1000

void *banco();
void *empleado();

int main() {
    pthread_t th1, th2;
    pthread_mutex_init(&mutex_saldo, NULL);
    pthread_cond_init(&cond_saldo_mayor1000, NULL);

    pthread_create(&th1, NULL, banco, NULL);
    pthread_create(&th2, NULL, empleado, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    pthread_mutex_destroy(&mutex_saldo);
    pthread_cond_destroy(&cond_saldo_mayor1000);
    
    return 0;
}

void *banco() {
    while (1) {
        sleep(5); // Simulacion llegada nomina
        pthread_mutex_lock(&mutex_saldo);
        saldo += 3000;  // Simulacion ingreso nomina
        pthread_cond_signal(&cond_saldo_mayor1000);  // Condicion de otro
        pthread_mutex_unlock(&mutex_saldo);
    }
}

void *empleado() {
    while (1) {
        pthread_mutex_lock(&mutex_saldo);
        printf("Empleado dentro de la seccion critica\n");

        while (saldo < CANTIDAD_A_RETIRAR) {  // Condicion que impide seguir
            printf("Empleado esperando a tener suficiente saldo...\n");
            pthread_cond_wait(&cond_saldo_mayor1000, &mutex_saldo);
            sleep(1);
        }

        saldo -= CANTIDAD_A_RETIRAR;
        printf("Saque %d, saldo: %d\n", CANTIDAD_A_RETIRAR, saldo);
        pthread_mutex_unlock(&mutex_saldo);

        sleep(2);
    }
}