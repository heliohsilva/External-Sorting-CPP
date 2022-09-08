/*
========================================================
arquivo de cabeçalho contendo a classe responsável
pela conversão do arquivo csv em binário
========================================================
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include "registro.hpp"
#ifndef CONVERSAO_HPP
#define CONVERSAO_HPP

using namespace std;

class arquivoCSV
{
private:
    dadosFuncionario *funcionario;
    ifstream arquivoEntrada;
    ofstream arquivoConvertido;

    bool verificar_se_string_esta_vazia(const char *palavra);
    string ler_palavra(ifstream &arquivoEntrada);
    void gravar_no_arquivo_binario(ofstream &arquivoConvertido);

public:
    arquivoCSV();
    ~arquivoCSV();
    void verificar_se_string_eh_numero(string &palavra);
    void ler_os_tipos_do_registro_em_cada_linha();
};

arquivoCSV::arquivoCSV()
{
    funcionario = new dadosFuncionario;
}
arquivoCSV::~arquivoCSV()
{
    delete funcionario;
}

bool arquivoCSV::verificar_se_string_esta_vazia(const char *palavra)
{
    return strlen(palavra) == 0; // retorna true se a string nao tiver dados, e false se tiver
}

string arquivoCSV::ler_palavra(ifstream &arquivoEntrada)
{
    string palavra;
    char caracterParaFormarAPalavra;
    arquivoEntrada.read(&caracterParaFormarAPalavra, 1); // le 1 caracter para realizar a verificaçao e entrar no looping

    if (caracterParaFormarAPalavra == '"') // quando tem mais d 1 dado string em 1 posiçao so, esses dados tem uma virgula entre eles, entao passa a usar as aspas como delimitdor
    {
        do
        {
            palavra += caracterParaFormarAPalavra; // adiciona o caracter lido à string
            arquivoEntrada.read(&caracterParaFormarAPalavra, 1);
        } while (caracterParaFormarAPalavra != '"');

        arquivoEntrada.read(&caracterParaFormarAPalavra, 1); // le 1 caracter por vez

        palavra += '"';
    }
    else
    {
        // caso a string n começa com aspas, entao será um dado so, entao o delimitador passa a ser a virgula, quebra de linha ou fim do arquivo
        do
        {
            palavra += caracterParaFormarAPalavra;
            arquivoEntrada.read(&caracterParaFormarAPalavra, 1);

        } while (caracterParaFormarAPalavra != ',' and caracterParaFormarAPalavra != '\n' and !arquivoEntrada.eof());
    }

    return palavra;
}

void arquivoCSV::gravar_no_arquivo_binario(ofstream &arquivoConvertido)
{
    arquivoConvertido.write((const char *)(funcionario), sizeof(dadosFuncionario));
}

void arquivoCSV::verificar_se_string_eh_numero(string &palavra)
{
    for (unsigned i = 0; i < (palavra.length()); ++i)
    {
        if (palavra[i] != '.' and isdigit(palavra[i]) == 0)
            palavra = '0'; // se a string nao for um número, o valor dela é substituído por 0
    }
}

void arquivoCSV::ler_os_tipos_do_registro_em_cada_linha()
{

    arquivoEntrada.open("san_francisco_payroll_dataset.csv");
    arquivoConvertido.open("binario.bin", ios::binary);

    string palavra;

    if (!arquivoEntrada.good())
    {
        cout << "ERRO!! Nao foi possivel abrir o arquivo csv";
    }
    else
    {

        for (int i = 0; i < 10; i++)
        {
            ler_palavra(arquivoEntrada); // elimina a primeira linha(cabeçalho)
        }

        while (!arquivoEntrada.eof())
        {
            palavra = ler_palavra(arquivoEntrada);
            if (!verificar_se_string_esta_vazia(palavra.data()))
            {
                // metodos stoi, stof e strcpy são responsáveis por converter variaveis do tipo string em int, float e char, respectivamente

                funcionario->id = stoi(palavra);
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                strcpy(funcionario->nomeFuncionario, palavra.c_str());
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                strcpy(funcionario->cargo, palavra.c_str());
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                verificar_se_string_eh_numero(palavra);
                funcionario->pagamentoBase = stof(palavra);
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                verificar_se_string_eh_numero(palavra);
                funcionario->pagamentoHoraExtra = stof(palavra);
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                verificar_se_string_eh_numero(palavra);
                funcionario->outroPagamento = stof(palavra);
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                verificar_se_string_eh_numero(palavra);
                funcionario->beneficios = stof(palavra);
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                verificar_se_string_eh_numero(palavra);
                funcionario->pagamentoTotal = stof(palavra);
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                verificar_se_string_eh_numero(palavra);
                funcionario->pagamentoTotalVezesBeneficios = stof(palavra);
                palavra.clear();

                palavra = ler_palavra(arquivoEntrada);
                funcionario->ano = stoi(palavra);
                palavra.clear();

                gravar_no_arquivo_binario(arquivoConvertido);
            }
        }
        cout << "Terminado!" << endl;

        arquivoConvertido.close();
        arquivoEntrada.close();
    }
}


#endif