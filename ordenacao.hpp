/*
========================================================
Arquivo de cabeçalho contendo a classe responsável
pela ordenação do arquivo binário
========================================================
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include "registro.hpp"

using namespace std;

class ordena
{
private:
    dadosFuncionario *funcionario = new dadosFuncionario; // funcionario principal
    dadosFuncionario *temp = new dadosFuncionario;        // funcionario 'auxiliar' para troca de dados
    fstream arq;
    int n;

public:
    ordena();
    ~ordena();
    void shellSort();
    int comparar(char *a, char *b); // funcao que compara 2 strings e retorna 1 se a>b, 0 se a=b e -1 se a<b, desconsiderando letras maiusculas e minusculas
};

ordena::ordena()
{
    arq.open("binario.bin", ios::binary | ios::in | ios::out);
    arq.seekg(sizeof(dadosFuncionario) * 0, ios::end); // aponta para o final do arquivo para pegar o numero de registros
    n = arq.tellg() / sizeof(dadosFuncionario);        // pega o numero de registros
}

ordena::~ordena()
{
    delete temp;
    delete funcionario;
}

void ordena::shellSort()
{
    for (int gap = n / 4; gap > 0; gap /= 4) // gap = tamanho do salto
    {
        for (int i = gap; i < n; i += 1) // percorre o sub bloco do arquivo do tamanho do gap
        {
            arq.seekg(sizeof(dadosFuncionario) * i, ios::beg);
            arq.read((char *)(temp), sizeof(dadosFuncionario));

            int j = i;

            arq.seekg(sizeof(dadosFuncionario) * (j - gap), ios::beg);
            arq.read((char *)(funcionario), sizeof(dadosFuncionario));

            // compara o registro i com o registro j-gap e se o registro i for menor que o registro j-gap, troca os registros, senao, se forem iguais compara o id
            while (j >= gap && (comparar(temp->nomeFuncionario, funcionario->nomeFuncionario) > 0 || (comparar(temp->nomeFuncionario, funcionario->nomeFuncionario) == 0 && temp->id > funcionario->id)))
            {
                arq.seekp(sizeof(dadosFuncionario) * j, ios::beg);
                arq.write((char *)(funcionario), sizeof(dadosFuncionario));

                j -= gap;

                if (j >= gap)
                {
                    arq.seekg(sizeof(dadosFuncionario) * (j - gap), ios::beg);
                    arq.read((char *)(funcionario), sizeof(dadosFuncionario));
                }
            }

            arq.seekp(sizeof(dadosFuncionario) * j, ios::beg);
            arq.write((char *)(temp), sizeof(dadosFuncionario));
        }
    }

    arq.close();

    cout << "Arquivo ordenado com sucesso!" << endl;
}

int ordena::comparar(char *a, char *b)
{
    // recebe dois ponteiros char, independendo de ser maiusculo ou minusculo, retorna 1 se a>b, 0 se a=b e -1 se a<b
    char *x = new char[strlen(a) + 1];
    char *y = new char[strlen(b) + 1];

    for (unsigned i = 0; i < strlen(a); i++)
    {
        x[i] = tolower(a[i]);
    }
    for (unsigned i = 0; i < strlen(b); i++)
    {
        y[i] = tolower(b[i]);
    }

    return strcmp(x, y);
}
