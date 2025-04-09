#include <pthread.h>
#include <stdio.h>

void *f_thread(void *arg);

pthread_mutex_t mutex; 

int contador = 0;

int main()
{
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[128];

    for (int i = 0; i < 128; i++) {
        pthread_create(&threads[i], NULL, f_thread, NULL);
        pthread_join(threads[i], NULL);
    }

    printf("Valor final do contador: %d\n", contador);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}

void *f_thread(void *arg)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 1000; i++) {
        contador++;
    };
    pthread_mutex_unlock(&mutex);
    return 0;
}
