#include <stdio.h>
#include <pthread.h>

#define NUM 10000
#define PASADAS 2000

pthread_mutex_t mutex_buf;
int buf[NUM];

int use_mutex = 0;
volatile int stop = 0; 

void *writer(void *arg);
void *reader(void *arg);
void lock_if_needed(void);
void unlock_if_needed(void);

int main() {
    pthread_t th_w, th_r;  // write and read threads

    use_mutex = (argc > 1);

    // Initialize mutex
    if (pthread_mutex_init(&mutex_buf, NULL) != 0) {
        perror("pthread_mutex_init");
        return 1;
    }

    // Create threads
    if (pthread_create(&th_w, NULL, writer, NULL) != 0) {
        perror("pthread_create writer");
        pthread_mutex_destroy(&mutex_buf);
        return 1;
    }
    if (pthread_create(&th_r, NULL, reader, NULL) != 0) {
        perror("pthread_create reader");
        pthread_mutex_destroy(&mutex_buf);
        return 1;
    }

    // Conect threads
    if (pthread_join(th_w, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }
    if (pthread_join(th_r, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    // Mutex destruction
    pthread_mutex_destroy(&mutex_buf);

    return 0;
}

void *writer(void *arg) {
    (void) arg;

    pthread_mutex_lock(&mutex_buf);
    for (int i = 0; i < 10000; i++) {
        buf[i] = NUM;
    }
    pthread_mutex_unlock(&mutex_buf);

    return NULL;
}

void *reader(void *arg) {
    (void) arg;

    pthread_mutex_lock(&mutex_buf);
    for (int i = 0; i < 10000; i++) {
        if (buf[i] == NUM) {
            printf("Correcto\n");
        }
    }
    pthread_mutex_unlock(&mutex_buf);


    return NULL;
}

void lock_if_needed(void) {
    if (use_mutex) pthread_mutex_lock(&mutex_buf);
}

void unlock_if_needed(void) {
    if (use_mutex) pthread_mutex_unlock(&mutex_buf);
}