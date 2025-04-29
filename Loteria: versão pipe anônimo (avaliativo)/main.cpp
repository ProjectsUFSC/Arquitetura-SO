#include <unistd.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>

using namespace std;

int main(){
    int nprocs = 10;
    pid_t pids[nprocs];
    int fd[2] {};
    int pid_sorteado;

    if(pipe(fd) == -1){
        cout << "!!!FALHA!!!" << endl << "Erro ao criar pipe." << endl;
        return 1;
    }


    for(int i = 0; i < nprocs; i++){
        pid_t pid = fork();
        if(pid < 0){
            cout << "!!!FALHA!!!" << endl << "Erro ao criar processo." << endl;
            return 1;
        }
        if(pid > 0){
            pids[i] = pid;
        }
        else if(pid == 0){
            read(fd[0], &pid_sorteado, sizeof(pid_sorteado));
            if(pid_sorteado == getpid()){
                cout << getpid() << ": fui sorteado " <<endl;
            }
            close(fd[0]);
            close(fd[1]);
            return 0;
        }

    }
    srand(time(nullptr));
    pid_t sorteado = pids[rand() % nprocs];
    cout << "PID sorteado: " << sorteado << endl;

    for(int i = 0; i < nprocs; i++){
        write(fd[1], &sorteado, sizeof(sorteado));
    }
    close(fd[0]);
    close(fd[1]);

    for(int i = 0; i < nprocs; i++){
        wait(nullptr);
    }


}
