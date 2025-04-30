#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;


int main(){

    int socket_fd{0}, connect_fd{0};

    struct sockaddr_in serv_addr {};

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd < 0){
        cout << "!!!FALHA!!!" << endl << "Erro ao criar socket." << endl;
        return 1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8080);

    if (::bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        cout << "!!!FALHA!!!" << endl << "Erro ao fazer bind." << endl;
        return 1;
    }

    if (listen(socket_fd, 10) < 0){
        cout << "!!!FALHA!!!" << endl << "Erro ao fazer listen." << endl;
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            cout << "Erro ao criar processo filho." << endl;
            return 1;
        } else if (pid == 0) {

            int client_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (client_fd < 0) {
                cout << "Erro ao criar socket no filho." << endl;
                exit(1);
            }

            if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                cout << "Erro ao conectar ao pai." << endl;
                exit(1);
            }

            pid_t meu_pid = getpid();
            write(client_fd, &meu_pid, sizeof(meu_pid));

            pid_t pid_sorteado;
            read(client_fd, &pid_sorteado, sizeof(pid_sorteado));

            if (meu_pid == pid_sorteado) {
                cout << meu_pid << ": fui sorteado" << endl;
            }

            close(client_fd);
            exit(0);
        }
    }

    pid_t pids[10];
    int sockets[10];
    int conectados = 0;

    while (conectados < 10) {
        connect_fd = accept(socket_fd, nullptr, nullptr);
        if (connect_fd < 0) {
            cout << "Erro ao aceitar conexão." << endl;
            continue;
        }

        pid_t pid_recebido;
        read(connect_fd, &pid_recebido, sizeof(pid_recebido));
        pids[conectados] = pid_recebido;
        sockets[conectados] = connect_fd;
        conectados++;
    }

    srand(time(nullptr));
    int sorteado_index = rand() % 10;
    pid_t pid_sorteado = pids[sorteado_index];

    cout << "PID sorteado: " << pid_sorteado << endl;

    for (int i = 0; i < 10; i++) {
        write(sockets[i], &pid_sorteado, sizeof(pid_sorteado));
        close(sockets[i]);
    }

    close(socket_fd);

    return 0;
}
