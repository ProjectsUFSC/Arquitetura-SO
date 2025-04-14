#include <stdio.h>
#include <pthread.h>

using namespace std;

volatile bool is_prime = false;

void* f_thread1(void* arg);
void* f_thread2(void* arg);

pthread_t t1, t2;

int main(){
    long x;
    scanf("%ld", &x);

    pthread_create(&t1, NULL, f_thread1, (void*)x);
    pthread_create(&t2, NULL, f_thread2, (void*)x);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    if(!is_prime){
        printf("1\n");
    }
    
    return 0;
}

void* f_thread1(void* arg){
    long x = (long) arg;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    
    for(int i = x/2; i < x; i++){
        if(x % i == 0 && i > 1){
            pthread_cancel(t2);
            if(!is_prime){
                is_prime = true;
                printf("0\n");
            }
            return 0 ;
        }
    }
    
    return 0;
}

void* f_thread2(void* arg){
    long x = (long) arg;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    
    for(int i = x/2; i > 1; i--){
        if(x % i == 0 && i > 1){
            pthread_cancel(t1);
            if(!is_prime){
                is_prime = true;
                printf("0\n");
            }
            return 0;
        }
    }
    
    return 0;
}