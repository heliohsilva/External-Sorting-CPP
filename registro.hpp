/*
========================================================
arquivo de cabeçalho contendo o registro utilizado
para ler os dados do arquivo
========================================================
*/

#include <iostream>
#ifndef REGISTRO_HPP
#define REGISTRO_HPP

struct dadosFuncionario
{
    int id;
    char nomeFuncionario[150], cargo[200];
    float pagamentoBase,
        pagamentoHoraExtra,
        outroPagamento;
    float beneficios;
    float pagamentoTotal,
        pagamentoTotalVezesBeneficios;
    int ano;
};

#endif