#include "Solucao.hpp"

Solucao::Solucao(vector<vector<int>> rotas, vector<int> terceirizados, int custoTotal, int custoTerceirizacao, int custoVeiculo, int custoRotas){
    this->rotas = rotas;
    this->terceirizados = terceirizados;
    this->custoTotal = custoTotal;
    this->custoTerceirizacao = custoTerceirizacao;
    this->custoVeiculo = custoVeiculo;
    this->custoRotas = custoRotas;
}