#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t resource;   
sem_t readTry;   
sem_t rmutex;
sem_t wmutex;     

int read_count = 0;
int write_count = 0;


void *reader(void *arg) {
    int id = *((int *)arg);

    
    sem_wait(&readTry);     
    sem_wait(&rmutex);
    read_count++;
    if (read_count == 1)
        sem_wait(&resource); 
    sem_post(&rmutex);
    sem_post(&readTry);


    printf("Reader %d is reading\n", id);
    sleep(1);

    
    sem_wait(&rmutex);
    read_count--;
    if (read_count == 0)
        sem_post(&resource); 
    sem_post(&rmutex);

    return NULL;
}


void *writer(void *arg) {
    int id = *((int *)arg);

    
    sem_wait(&wmutex);
    write_count++;
    if (write_count == 1)
        sem_wait(&readTry); 
    sem_post(&wmutex);

    sem_wait(&resource);

    
    printf("Writer %d is writing\n", id);
    sleep(1);

    sem_post(&resource);

    
    sem_wait(&wmutex);
    write_count--;
    if (write_count == 0)
        sem_post(&readTry); 
    sem_post(&wmutex);

    return NULL;
}

int main() {
    pthread_t r[5], w[5];
    int ids[5];

    
    sem_init(&resource, 0, 1);
    sem_init(&readTry, 0, 1);
    sem_init(&rmutex, 0, 1);
    sem_init(&wmutex, 0, 1);

    
    for (int i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &ids[i]);
    }

    
    for (int i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(r[i], NULL);
        pthread_join(w[i], NULL);
    }

    return 0;
}