#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

volatile char filosofos[5] = {'P','P','P','P','P'};

sem_t semaforos[5];

pthread_mutex_t mutex;

void* f_mutex(void* arg){
    pthread_mutex_lock(&mutex);
    long i = (long) arg;

    if(filosofos[i]== 'P'){
        printf("Filosofo %ld pegou o garfo\n", i);
        filosofos[i] = 'F';
        
    } else if (filosofos[i] == 'F' && filosofos[(i+4)%5] != 'C' && filosofos[(i+1)%5] != 'C'){
        printf("Filosofo %ld está comendo\n", i);
        filosofos[i] = 'C';
        sem_post(&semaforos[i]);
        sem_post(&semaforos[(i+4)%5]);
        sem_post(&semaforos[(i+1)%5]);

    } else if(filosofos[i]== 'F'){
        sem_wait(&semaforos[i]);
    } else {
        printf("Filosofo %ld largou o garfo\n", i);
        filosofos[i] = 'P';

    }

    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

void* f_filosofos(void* arg){
    
    while(1){
        long i = (long) arg;
        int tempo = rand() % 5+1;
        sleep(tempo);

        f_mutex((void*) i);
    }


}

void* f_print(void* arg){
    while(1){
        for(int i = 0; i < 5; i++){
            printf("%c ", filosofos[i]);
        }
        printf("\n");
        sleep(2);
    }
}

int main(){
    srand(time(NULL));
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


// #include <stdio.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>
// #include <stdlib.h>

// volatile char filosofos[5] = {'P','P','P','P','P'};

// sem_t semaforos[5];

// pthread_mutex_t mutex;

// void* f_mutex(void* arg){
//     pthread_mutex_lock(&mutex);
//     long i = (long) arg;

//     if(filosofos[i]== 'P'){
//         filosofos[i] = 'F';
//         printf("Filosofo %ld pegou o garfo\n", i);
//     } else if (filosofos[i] == 'F' && filosofos[(i+4)%5] != 'C' && filosofos[(i+1)%5] != 'C'){
//         printf("Filosofo %ld está comendo\n", i);
//         filosofos[i] = 'C';

//     } else {
//         printf("Filosofo %ld largou o garfo\n", i);
//         filosofos[i] = 'P';

//     }

//     pthread_mutex_unlock(&mutex);
//     return NULL;
// }

// void* f_filosofos(void* arg){
    
//     while(1){
//         long i = (long) arg;
//         int sleep = rand() % 1000000;
//         usleep(sleep);

//         f_mutex((void*) i);
//     }


// }

// void* f_print(void* arg){
//     while(1){
//         for(int i = 0; i < 5; i++){
//             printf("%c ", filosofos[i]);
//         }
//         printf("\n");
//         usleep(90000);
//     }
// }

// int main(){

//     pthread_t t1, t2, t3, t4, t5, t6;

//     pthread_mutex_init(&mutex, NULL);

//     for(int i = 0; i < 5; i++){
//         sem_init(&semaforos[i], 0, 0);
//     }

//     pthread_create(&t1, NULL, f_filosofos, (void*) 0);
//     pthread_create(&t2, NULL, f_filosofos, (void*) 1);
//     pthread_create(&t3, NULL, f_filosofos, (void*) 2);
//     pthread_create(&t4, NULL, f_filosofos, (void*) 3);
//     pthread_create(&t5, NULL, f_filosofos, (void*) 4);
//     pthread_create(&t6, NULL, f_print, NULL);

//     pthread_join(t1,NULL);
//     pthread_join(t2,NULL);
//     pthread_join(t3,NULL);
//     pthread_join(t4,NULL);
//     pthread_join(t5,NULL);
//     pthread_join(t6,NULL);


//     for(int i = 0; i < 5; i++){
//         sem_destroy(&semaforos[i]);
//     }

//     pthread_mutex_destroy(&mutex);

//     return 0;


// }



