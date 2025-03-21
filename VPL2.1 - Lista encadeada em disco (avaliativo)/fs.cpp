#ifndef fs_h
#define fs_h
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

/**
 * @param arquivoDaLista nome do arquivo em disco que contem a lista encadeada
 * @param novoNome nome a ser adicionado apos depoisDesteNome
 * @param depoisDesteNome um nome presente na lista
 */
void adiciona(std::string arquivoDaLista, std::string novoNome, std::string depoisDesteNome)
{
    fstream file;
    file.open(arquivoDaLista, ios::in | ios::out | ios::binary);
    if(file.is_open())
    {
        cout << "Arquivo aberto com sucesso" << endl;
    }
    else
    {
        cout << "Erro ao abrir arquivo" << endl;
        return;
    }

    // leitura do inicio da lista de nodos
    int position = 0;
    file.seekg(position, file.beg);
    file.read((char *)&position, sizeof(position));
    cout << "Inicio da lista: " << position << endl << endl;


    // loop para ler os nodos, caso negativo acabou a lista
    while(position > 0 )
    {
        // vai para a posição incial 
        file.seekg(position, file.beg);

        // vai para a parte de nome do nodo         
        file.seekg(position + 4, file.beg);
        char name[20];
        file.read((char *)&name, sizeof(name));
        cout << "Nome: " << name << endl;

        // vai para a parte de proximo nodo
        file.seekg(position + 24, file.beg);
        int next_position;
        file.read((char *)&next_position, sizeof(next_position));
        cout << "Posição próximo nome: " << next_position << endl << endl;

        if(string(name) == depoisDesteNome)
        {
            cout << "!!!Nome encontrado!!!" << endl;

            // procura espaço vazio ( nodo não listado) para adicionar o novo nome
            for(int i = 4; i < file.tellg(); i+=28)
            {
                int is_listed;
                file.seekg(i, file.beg);
                file.read((char *)&is_listed, sizeof(is_listed));
                if(is_listed == 0)
                {
                    cout << "Espaço de " << i << " a " << i+28 << " vazio" << endl;

                    // adicionando valor de listado para o novo nodo
                    file.seekg(i, file.beg);
                    int listed_name = 1;
                    file.write((char *)&listed_name, sizeof(listed_name));

                    // adicionando nome ao novo nodo
                    char Buffer[20];
                    strcpy(Buffer, novoNome.c_str()); // convertendo string para char
                    file.seekg(i + 4, file.beg);
                    file.write((char *)&Buffer, sizeof(Buffer));

                    // adicionando posição do proximo nodo ao novo nodo
                    file.seekg(i + 24, file.beg);
                    file.write((char *)&next_position, sizeof(next_position));

                    cout << "Nome adicionado: " << novoNome << endl;

                    // adicionando posição do novo nodo ao nodo anterior para n perder referencia
                    file.seekg(position + 24, file.beg);
                    file.write((char *)&i, sizeof(i));
                    file.close();
                    return;
                }
            }
        }
        position = next_position;
    }
    cout << "Nome não encontrado" << endl;
    file.close();
    return;
}

#endif /* fs_h */