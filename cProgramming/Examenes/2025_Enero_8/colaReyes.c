#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
# include <signal.h>

//#define NINOS 15
static int n_ninos = 0;


//#define REYES 3
static int reyes = 3;
pthread_mutex_t m_reyes;
pthread_cond_t c_reyes;

void *reyMago(void *num);
void manejador (int sig);

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "USO: %s N_NINOS\n", argv[0]);
        return 1;
    }
    
    n_ninos = atoi(argv[1]);

    // comprobacion del argumento
    if (n_ninos <= 0) {
        fprintf(stderr, "Parametro debe ser mayor que 0\n");
        return 1;
    }

    pthread_t *thread = malloc(sizeof(pthread_t) * (size_t)n_ninos);
    int *id_nino = malloc(sizeof(int) * (size_t)n_ninos);
    if (!thread || !id_nino) {
        perror("malloc");
        free(thread);
        free(id_nino);
        return 1;
    }

    if(pthread_mutex_init(&m_reyes, NULL) != 0) {
        perror("pthread_mutex_init");
        free(thread);
        free(id_nino);
        return 1;
    }

    if(pthread_cond_init(&c_reyes, NULL) != 0) {
        perror("pthread_cond_init");
        free(thread);
        free(id_nino);
        return 1;
    }

    for (int i = 0; i < n_ninos; i++) {

        id_nino[i] = i + 1;

        if (pthread_create(&thread[i], NULL, reyMago, (void *)&id_nino[i]) != 0) {
            perror("pthread_create");
            free(thread);
            free(id_nino);
            return 1;
        }
    }

    for (int i = 0; i < n_ninos; i++) {
        if (pthread_join(thread[i], NULL) != 0) {
            perror("pthread_join");
            free(thread);
            free(id_nino);
            return 1;
        }
    }

    printf("Se ha terminado la cola de ninos\n");

    pthread_cond_destroy(&c_reyes);
    pthread_mutex_destroy(&m_reyes);

    free(thread);
    free(id_nino);

    return 0;
}


void *reyMago(void *num) {
    int id = *(int *) num;
    signal(SIGINT, manejador);
    
    while (1) {
        printf("Nino %d esperando en la cola\n", id);

        pthread_mutex_lock(&m_reyes);
        while (reyes == 0) {
            pthread_cond_wait(&c_reyes, &m_reyes);
        }
        reyes--;
        pthread_mutex_unlock(&m_reyes);

        printf("Nino %d esta siendo atentido por un Rey Mago\n", id);
        sleep(1 + (rand() % 5));
        printf("Nino %d entrega su carta al Rey Mago\n", id);
        
        pthread_mutex_lock(&m_reyes);
        reyes++;
        pthread_cond_signal(&c_reyes);
        pthread_mutex_unlock(&m_reyes);

        printf("Nino %d espera para volver a la cola\n", id);
        sleep(1 + (rand() % 5));
    }

    return NULL;
}

void manejador (int sig)
{
    printf("\nRecibida senial %d\n", sig);
}
