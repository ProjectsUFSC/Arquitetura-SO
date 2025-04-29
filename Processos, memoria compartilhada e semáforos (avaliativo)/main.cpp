#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int id = 0;

int main(){

    srand(time(nullptr));

    int nprocs;
    cout << "Digite o número deprocessos que deseja criar:" << endl;
    cin >> nprocs;

    if(nprocs <= 0){
        cout << "!!!FALHA!!!" << "Número de processos inválido." << endl << "Use um número maior que 0." << endl;
        return 1;
    }

    int *id = (int *)mmap(nullptr, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (id == MAP_FAILED) {
        cout << "!!!FALHA!!!" << endl << "Erro ao criar memória compartilhada." << endl;
        return 1;
    }
    *id = 0;

    sem_t *semaforo = sem_open("/semaforo", O_CREAT , 0644, 1);
    if(semaforo == SEM_FAILED){
        cout << "!!!FALHA!!!" << endl << "Erro ao criar semáforo." << endl;
        sem_close(semaforo);
        sem_unlink("/semaforo");
        munmap(id, sizeof(int));
        return 1;
    }

    for(int i = 0; i < nprocs; i++){
        int tempo = rand() % 10 + 1;
        sleep(tempo);
        pid_t pid = fork();

        if(pid == 0 ){
            // int tempo = rand() % 10 + 1;
            // sleep(tempo);
            sem_wait(semaforo);
            *id += 1;
            cout << "Processo " << *id << " criado" << endl;
            sem_post(semaforo);

            return 0;
        }

    }

    for(int i = 0; i < nprocs; i++){
        wait(NULL);
    }

    sem_close(semaforo);
    sem_unlink("/mysemaphore");
    munmap(id, sizeof(int));

    return 0;
}