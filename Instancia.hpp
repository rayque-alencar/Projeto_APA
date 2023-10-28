#ifndef INSTANCIA_HPP
#define INSTANCIA_HPP

#include <vector>
#include <string> 
#include <iostream>
#include <fstream>

using namespace std;

// Classe que representa a instancia do problema
class Instancia {
    public:
        int nEntregas, nVeiculos, capacidadeVeiculo, minEntregas, custoVeiculo; 
        vector<int> demandas, custosTerceirizacao; 
        vector<vector<int>> custo;

        Instancia(const string& nomeArquivo);
        void imprimirDados();
};

#endif
