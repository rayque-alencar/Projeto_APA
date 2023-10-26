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
        int num_de_Entregas, num_De_Veiculos, capacidade_Veiculo, min_De_Entregas, custo_Do_Veiculo; 
        vector<int> demadas, custos_De_Terceirizacao; 
        vector<vector<int>> custo;

        Instancia(const string& nomeArquivo);
        void imprimirDados();
};

#endif
