#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

volatile char filosofos[5] = {'P','P','P','P','P'};

sem_t semaforos[5];

pthread_mutex_t mutex;

void* f_mutex(void* arg){
    pthread_mutex_lock(&mutex);
    long i = (long) arg;

    if(filosofos[i]== 'P'){
        filosofos[i] = 'F';
        sem_wait(&semaforos[i]);
    
    } else if (filosofos[i] == 'F' && filosofos[(i+4)%5] != 'C' && filosofos[(i+1)%5] != 'C'){
        filosofos[i] = 'C';
        sem_post(&semaforos[i]);

    } else {
        filosofos[i] = 'P';

    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* f_filosofos(void* arg){
    
    while(1){
        long i = (long) arg;
        int sleep = rand() % 10000;
        usleep(sleep);

        f_mutex((void*) i);
    }


}

void* f_print(void* arg){
    while(1){
        for(int i = 0; i < 5; i++){
            printf("%c ", filosofos[i]);
        }
        printf("\n");
        usleep(8000);
    }
}

int main(){

    pthread_t t1, t2, t3, t4, t5, t6;

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < 5; i++){
        sem_init(&semaforos[i], 0, 0);
    }

    pthread_create(&t1, NULL, f_filosofos, (void*) 0);
    pthread_create(&t2, NULL, f_filosofos, (void*) 1);
    pthread_create(&t3, NULL, f_filosofos, (void*) 2);
    pthread_create(&t4, NULL, f_filosofos, (void*) 3);
    pthread_create(&t5, NULL, f_filosofos, (void*) 4);
    pthread_create(&t6, NULL, f_print, NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);
    pthread_join(t5,NULL);
    pthread_join(t6,NULL);


    for(int i = 0; i < 5; i++){
        sem_destroy(&semaforos[i]);
    }

    pthread_mutex_destroy(&mutex);

    return 0;


}



