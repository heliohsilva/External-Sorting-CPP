/*
========================================================
arquivo de cabeçalho contendo a classe responsável
pelas operaçoes de manipulação do arquivo binário
========================================================
*/

#include <iostream>
#include <fstream>
#include "registro.hpp"
#include "conversao.hpp"
#include "ordenacao.hpp"

using namespace std;

class arquivoBIN
{
private:
    fstream arquivo;
    string nomeDoArquivo = "binario.bin";
    dadosFuncionario *funcionario;
    string mensagemParaFalhaDeAberturaDoArquivo = "\nERRO ao abrir o arquivo!!! \n\n";
    ordena *ordenaArquivo = new ordena;

    void opcoes_de_escolha_envolvendo_registros();
    void adicionar_elemento_em_uma_posicao_especifica();
    void listar_dados_entre_duas_posicoes();
    void alterar_dados_de_uma_posicao();
    void trocar_dois_registros_de_posicao();
    void listar_dados();
    void setar_dados_em_um_registro(int posicao); // define o id baseado na posicao a ser inserida e le o restante dos dados. metodo utilizado como auxiliar do alterar_dados e do adicionar_elemento
    void imprimir(int comeco, int fim);

public:
    arquivoBIN();
    ~arquivoBIN();
    void menu_principal();
};

arquivoBIN::arquivoBIN()
{
    funcionario = new dadosFuncionario;
}

arquivoBIN::~arquivoBIN()
{
    delete funcionario;
    delete ordenaArquivo;
}

void arquivoBIN::menu_principal()
{
    arquivoCSV paraconverter;

    int opcao;

    do
    {
        cout << "===============================================================================" << endl
             << "1 - Converter o arquivo csv para binario" << endl
             << "2 - Exibir opcoes de manipulacao do arquivo binario" << endl
             << "3 - Finalizar o programa" << endl;
        cout << "===============================================================================" << endl;
        cin >> opcao;
        switch (opcao)
        {
        case 1:
            paraconverter.ler_os_tipos_do_registro_em_cada_linha();
            break;
        case 2:
            opcoes_de_escolha_envolvendo_registros();
            break;
        case 3:
            exit(0);
            break;
        }
    } while (opcao != 3);
}

void arquivoBIN::opcoes_de_escolha_envolvendo_registros()
{
    int opcao;

    while (opcao != 7)
    {
        cout << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "1 - Adicionar um registro" << endl
             << "2 - Imprimir dados entre 2 posicoes" << endl
             << "3 - Alterar algum registro" << endl
             << "4 - Trocar 2 registros entre si" << endl
             << "5 - Listar dados" << endl
             << "6 - Ordenar por nome do funcionario" << endl
             << "7 - Retornar ao menu anterior" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            adicionar_elemento_em_uma_posicao_especifica();
            break;
        case 2:
            listar_dados_entre_duas_posicoes();
            break;
        case 3:
            alterar_dados_de_uma_posicao();
            break;
        case 4:
            trocar_dois_registros_de_posicao();
            break;
        case 5:
            listar_dados();
            break;
        case 6:
            ordenaArquivo->shellSort();
            break;
        case 7:
            menu_principal();
            break;
        }
    }
}

void arquivoBIN::adicionar_elemento_em_uma_posicao_especifica()
{

    arquivo.open(nomeDoArquivo, ios::binary | ios::out | ios::in);

    if (!arquivo.is_open())
    {
        cout << mensagemParaFalhaDeAberturaDoArquivo;
    }
    else
    {
        int posicaoDeAdicaoDoElemento, posicaoAtual;

        cout << "posicao na qual deseja inserir o registro: ";
        cin >> posicaoDeAdicaoDoElemento;

        arquivo.seekg(sizeof(dadosFuncionario) * 0, ios::end);           // aponta para o fim do arquivo
        posicaoAtual = (arquivo.tellg() / sizeof(dadosFuncionario)) - 1; // tellg retorna a posição atual do arquivo em bytes. dividindo o valor pelo tamanho ocupado por registro, temos a quantidade de registros gravados no arquivo

        while (posicaoAtual > posicaoDeAdicaoDoElemento - 1)
        {
            arquivo.seekg(sizeof(dadosFuncionario) * posicaoAtual, ios::beg); // aponta para a posição atual a partir do inicio do arquivo
            arquivo.read((char *)&funcionario[0], sizeof(dadosFuncionario));
            funcionario->id++;
            arquivo.write((char *)(funcionario), sizeof(dadosFuncionario));
            posicaoAtual--;
        }

        arquivo.seekg(sizeof(dadosFuncionario) * posicaoDeAdicaoDoElemento, ios::beg);
        setar_dados_em_um_registro(posicaoDeAdicaoDoElemento);
        arquivo.write((char *)(funcionario), sizeof(dadosFuncionario));

        arquivo.close();
    }
}

void arquivoBIN::listar_dados_entre_duas_posicoes()
{
    int posicao1, posicao2;

    cout << "informe posicao 1 e posicao 2: ";
    cin >> posicao1 >> posicao2;

    imprimir(posicao1, posicao2 + 1);
}

void arquivoBIN::alterar_dados_de_uma_posicao()
{
    arquivo.open(nomeDoArquivo, ios::binary | ios::in | ios::out);

    if (!arquivo.is_open())
    {
        cout << mensagemParaFalhaDeAberturaDoArquivo;
    }
    else
    {
        int posicao;

        cout << "posicao para alteraçao: ";
        cin >> posicao;

        setar_dados_em_um_registro(posicao);

        arquivo.seekp(sizeof(dadosFuncionario) * posicao, ios::beg);

        arquivo.write((char *)(funcionario), sizeof(dadosFuncionario));

        arquivo.close();
    }
}

void arquivoBIN::trocar_dois_registros_de_posicao()
{
    arquivo.open(nomeDoArquivo, ios::binary | ios::in | ios::out);

    if (!arquivo.is_open())
    {
        cout << mensagemParaFalhaDeAberturaDoArquivo;
    }
    else
    {
        int posicao1, posicao2;
        dadosFuncionario *aux = new dadosFuncionario;

        cout << "posicoes para troca: ";
        cin >> posicao1 >> posicao2;

        // le o registro na primeira posição em funcionario
        arquivo.seekg(sizeof(dadosFuncionario) * posicao1, ios::beg);
        arquivo.read((char *)&funcionario[0], sizeof(dadosFuncionario));
        funcionario->id = posicao2;

        // le o registro na segunda posição em aux
        arquivo.seekg(sizeof(dadosFuncionario) * posicao2, ios::beg);
        arquivo.read((char *)&aux[0], sizeof(dadosFuncionario));

        // grava funcionario na segunda posição
        arquivo.seekp(sizeof(dadosFuncionario) * posicao2, ios::beg);
        arquivo.write((char *)(funcionario), sizeof(dadosFuncionario));
        aux->id = posicao1;

        // grava aux na primeira posição
        arquivo.seekp(sizeof(dadosFuncionario) * posicao1, ios::beg);
        arquivo.write((char *)(aux), sizeof(dadosFuncionario));

        arquivo.close();
    }
}

void arquivoBIN::listar_dados()
{
    int posicaoFinalArquivo;

    arquivo.open(nomeDoArquivo, ios::binary | ios::in);
    arquivo.seekg(0 * sizeof(dadosFuncionario), ios::end);
    posicaoFinalArquivo = arquivo.tellg() / sizeof(dadosFuncionario);
    arquivo.close();

    imprimir(0, posicaoFinalArquivo);
}

void arquivoBIN::setar_dados_em_um_registro(int posicao)
{
    string aux;
    arquivoCSV verifica;

    funcionario->id = posicao;

    cout << endl
         << "nome: ";
    cin.ignore();
    getline(cin, aux);
    strcpy(funcionario->nomeFuncionario, aux.c_str());
    aux.clear();

    cout << endl
         << "cargo: ";
    cin.ignore(0, '\n');
    getline(cin, aux);
    strcpy(funcionario->cargo, aux.c_str());
    aux.clear();

    cout << endl
         << "pagamento base: ";
    cin >> aux;
    verifica.verificar_se_string_eh_numero(aux);
    funcionario->pagamentoBase = stof(aux);
    aux.clear();

    cout << endl
         << "pagamento hora extra: ";
    cin >> aux;
    verifica.verificar_se_string_eh_numero(aux);
    funcionario->pagamentoHoraExtra = stof(aux);
    aux.clear();

    cout << endl
         << "outro pagamento: ";
    cin >> aux;
    verifica.verificar_se_string_eh_numero(aux);
    funcionario->outroPagamento = stof(aux);
    aux.clear();

    cout << endl
         << "beneficio: ";
    cin >> aux;
    verifica.verificar_se_string_eh_numero(aux);
    funcionario->beneficios = stof(aux);
    aux.clear();

    funcionario->pagamentoTotal = funcionario->pagamentoBase + funcionario->outroPagamento + funcionario->pagamentoHoraExtra;
    funcionario->pagamentoTotalVezesBeneficios = funcionario->beneficios + funcionario->pagamentoTotal;

    cout << endl
         << "ano: ";
    cin >> aux;
    verifica.verificar_se_string_eh_numero(aux);
    funcionario->ano = stoi(aux);
    aux.clear();
}

void arquivoBIN::imprimir(int inicio, int fim)
{
    arquivo.open(nomeDoArquivo, ios::binary | ios::in);

    if (!arquivo.is_open())
    {
        cout << mensagemParaFalhaDeAberturaDoArquivo;
    }
    else
    {
        int i = inicio;

        arquivo.seekg(sizeof(dadosFuncionario) * inicio, ios::beg);

        do
        {
            arquivo.read((char *)&funcionario[0], sizeof(dadosFuncionario));

            cout << funcionario->id << " | "
                 << funcionario->nomeFuncionario << "  "
                 << funcionario->cargo << "  "
                 << funcionario->pagamentoBase << "  "
                 << funcionario->pagamentoHoraExtra << "  "
                 << funcionario->outroPagamento << "  "
                 << funcionario->beneficios << "  "
                 << funcionario->pagamentoTotal << "  "
                 << funcionario->pagamentoTotalVezesBeneficios << "  "
                 << funcionario->ano << endl;
            i++;

        } while (i < fim);

        arquivo.close();
    }
}
