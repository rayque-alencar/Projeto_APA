#ifndef INSTANCIA_HPP
#define INSTANCIA_HPP

#include <vector>
#include <string> 
#include <iostream>
#include <fstream>

using namespace std;

// Classe que representa a instancia do problema
class Instancia {
    private:
        vector<vector<int>> rotas; 
        vector<bool> entregasAlocadas;
    public:
        int num_de_Entregas, num_De_Veiculos, capacidade_Veiculo, min_De_Entregas, custo_Do_Veiculo; 
        vector<int> demadas, custos_De_Terceirizacao; 
        vector<vector<int>> custo;
        int INT_MAX = 2147483647;

        Instancia(const string& nomeArquivo);
        void imprimirDados();
        void algoritmoGuloso();
        void imprimirRotas();
        int calcularCustoRota(const vector<int>& rota);
        int calcularCustoTerceirizacao(int entrega);
        //void Vizinhanca1();
};

#endif
