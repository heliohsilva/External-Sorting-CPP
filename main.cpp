/*
========================================================
Hélio Henrique Medeiros Silva

Base de dados: san_francisco_payroll_database.csv
========================================================
*/

#include <iostream>
#include <iomanip>
#include "manipulacao.hpp"

using namespace std;

int main()
{
    cout << setprecision(8); // define a quantidade de algarismos a serem exibidos em uma impressao d inteiro ou float

    arquivoBIN binario;

    binario.menu_principal();

    return 0;
}