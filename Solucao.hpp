#ifndef SOLUCAO_HPP
#define SOLUCAO_HPP

#include <vector>
#include <string> 
#include <iostream>
#include <fstream>

using namespace std;
class Solucao
{
    public:
        int custoTotal, custoTerceirizacao, custoVeiculo, custoRotas;
        vector<vector<int>> rotas;
        vector<int> terceirizados;

        Solucao(vector<vector<int>> rotas, vector<int> terceirizados, int custoTotal, int custoTerceirizacao, int custoVeiculo, int custoRotas);
    
    private:
    /* data */
};



#endif