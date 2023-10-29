#include "Instancia.hpp"
#include "Solucao.hpp"
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

typedef struct
{
    int m_v = 0; // indice do melhor carro
    int m_i = 0; // indice do primeiro cara para trocar
    int m_j = 0; // indice do segundo cara para trocar
    int m_custo = 9999999;
}t_swap; 

typedef struct
{
    int m_v_a = 0; // indice do primeiro veiculo
    int m_v_b = 0; // indice do segundo veiculo
    int m_i = 0;   // indice do cara a ser trocado do veiculo A para o veiculo B
    int m_j = 0;   // indice do cara a ser trocado do veiculo B para o veiculo A
    int m_custo = 9999999; // custo da troca
}x_swap; 

typedef struct
{
    int m_v = 0; // indice do melhor carro
    int m_i = 0; // indice do primeiro cara para trocar
    int m_ter = 0; // indice do terceirizado
    
    int m_custo_rotas = 9999999;
    int m_custo_ter = 9999999;
    int m_custo_veiculo = 0;

}t_terceirizado;

typedef struct
{
    int m_v = 0; // indice do melhor carro
    int m_i = 0; // indice do primeiro cara para trocar
    
    int m_custo_rotas = 9999999;
    int m_custo_ter = 9999999;
    int m_custo_veiculo = 0;

}i_terceirizado;

//função que calcula o custo das rotas
int custoRotas(vector<vector<int>> rotas, vector<vector<int>> custo){
    int custoRotas = 0;
    for(vector<int> rota : rotas){
        for (int i = 0; i < int(rota.size())-1; i++){
            custoRotas += custo[rota[i]][rota[i+1]];
        }
    }
    return custoRotas;
}

//função que calcula o custo da terceirização
int custoTerceirizacao(vector<int> terceirizados, vector<int> custosTerceirizacao){
    
    int custoTerceirizacao = 0;
    for (int i = 0; i < int(terceirizados.size()); i++){
        custoTerceirizacao += custosTerceirizacao[terceirizados[i]-1];
    }
    return custoTerceirizacao;
}

//função que calcula o custo de ultilização dos veiculos
int custoVeiculos(int custoVeiculo, vector<vector<int>> rotas){
   
    int custoVeiculos = 0;
    for(vector<int> rota : rotas){
        if (rota[0] != rota[1]){
            custoVeiculos += custoVeiculo;
        }
    }
    return custoVeiculos;
}

int calculaQuantidadeEntregas(vector<vector<int>> rotas){
    int quantidadeEntregas = 0;
    for (vector<int> rota : rotas){
        quantidadeEntregas += int(rota.size()) - 2;
    }
    return quantidadeEntregas;
}
//função que imprime a solução do algoritmo guloso
void imprimirSolucao(Solucao solucao){

    cout << "Custo Total: " << solucao.custoTotal << endl;
    cout << "Custo das rotas: " << solucao.custoRotas << endl;
    cout << "Custo de ultilizacao dos veiculos: " << solucao.custoVeiculo << endl;
    cout << "Custo da terceirizacao: " << solucao.custoTerceirizacao << endl;

    cout << endl;

    cout << "Clientes Terceirizados: ";
    for (int i = 0; i < int(solucao.terceirizados.size()); i++){
        cout << solucao.terceirizados[i] << " ";
    }

    cout << endl;

    int nRotas = 0;
    for(vector<int> rota : solucao.rotas){
        if (rota[0] != rota[1]){
            nRotas += 1;
        }
    }

    cout << "\nNumero de rotas: " << nRotas << endl;

    cout << endl;

    for (int i = 0; i < int(solucao.rotas.size()); i++){
        
        if(solucao.rotas[i].size() > 2){

            cout << "Rota " << i + 1 << ": ";

            for (int j = 1; j < int(solucao.rotas[i].size())- 1; j++){
                cout << solucao.rotas[i][j] << " ";
            }
            cout << endl;
        }
    }
    cout << endl;
}
//função que salva a solução do guloso em arquivo em um arquivo
void salvarSolucaoEmArquivo(const Solucao& solucao, std::ofstream& arquivo) {

    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return;
    }

    arquivo << solucao.custoTotal << endl;
    arquivo << solucao.custoRotas << endl;
    arquivo << solucao.custoVeiculo << endl;
    arquivo << solucao.custoTerceirizacao << endl;

    arquivo << endl;

    for (int i = 0; i < int(solucao.terceirizados.size()); i++) {
        arquivo << solucao.terceirizados[i] << " ";
    }

    arquivo << endl;

    int nRotas = 0;
    for (vector<int> rota : solucao.rotas) {
        if (rota[0] != rota[1]) {
            nRotas += 1;
        }
    }

    arquivo << endl;
    arquivo << nRotas << endl;

    for (int i = 0; i < int(solucao.rotas.size()); i++) {
        if (solucao.rotas[i].size() > 2) {
            for (int j = 1; j < int(solucao.rotas[i].size())- 1; j++) {
                arquivo << solucao.rotas[i][j] << " ";
            }
            arquivo << endl;
        }
    }
}
//função que calcula a capacidade de cada rota
vector<int> calculaCapacidadeRota(vector<int> demandas, vector<vector<int>> rotas){
    vector<int> capacidadeRota(rotas.size(), 0);
    for (int i = 0; i < int(rotas.size()); i++){
        for (int j = 0; j < int(rotas[i].size()); j++){
            capacidadeRota[i] += demandas[rotas[i][j]-1];
        }
    }
    return capacidadeRota;
}
//função que implementa o algoritmo guloso
Solucao guloso(int nVeiculos, int nEntregas, int capacidadeVeiculo, int minEntregas,int custoVeiculo, vector<int> demandas, vector<vector<int>> custo, vector<int> custosTerceirizacao){

    //inicialização das variaveis 
    vector<vector<int>> rotas(nVeiculos);
    vector<int> terceirizacao;
    vector<int> pesos(nVeiculos, 0);
    vector<int> candidatos(nEntregas);

    //inicialização do vetor de candidatos
    for (int i = 1; i <= nEntregas; i++){
        candidatos[i-1] = i;
    }

    //inicialização das rotas
    for (int v = 0; v < nVeiculos; v++){
        rotas[v].push_back(0);
        pesos.push_back(0);
    }

    //algoritmo guloso  
    while (!candidatos.empty()){
        int iMelhorCandidato = 0;
        int melhorV = 0;
        int melhorCusto = 999999999;

        //encontra o melhor candidato, quer é o que tem o menor custo de arco
        for(int i = 0 ; i < int(candidatos.size()); i++){
            for (int v = 0; v < nVeiculos; v++){
                int custoArco;

        
                if (rotas[v].back() == 0){
                    custoArco = custo[rotas[v].back()][candidatos[i]]+custoVeiculo;
                }else{
                    custoArco = custo[rotas[v].back()][candidatos[i]];
                }

                //custoArco = custo[rotas[v].back()][candidatos[i]];

                //verifica se o candidato é melhor que o melhor candidato atual e se o veiculo tem capacidade para atender o candidato
                if (custoArco < melhorCusto && pesos[v] + demandas[candidatos[i]-1] <= capacidadeVeiculo){
                    iMelhorCandidato = i;
                    melhorCusto = custoArco;
                    melhorV = v;
                }
            }
        }

        int melhorCandidato = candidatos[iMelhorCandidato];

        if (nEntregas-int(candidatos.size()) > minEntregas && custosTerceirizacao[melhorCandidato] < melhorCusto){
            terceirizacao.push_back(melhorCandidato);
        }
        else{
            rotas[melhorV].push_back(melhorCandidato);
            pesos[melhorV] += demandas[melhorCandidato-1];
        }
        candidatos.erase(candidatos.begin()+iMelhorCandidato);
    }

    //adiciona o deposito no final de cada rota
    for (int v = 0; v < nVeiculos; v++){
        rotas[v].push_back(0);
    }

    //calcula o custo da solução
    int custoRotasSolucao = custoRotas(rotas, custo);
    int custoTerceirizacaoSolucao = custoTerceirizacao(terceirizacao, custosTerceirizacao);
    int custoVeiculosSolucao = custoVeiculos(custoVeiculo, rotas);
    int custoTotal = custoRotasSolucao + custoTerceirizacaoSolucao + custoVeiculosSolucao;

    //cria a solução
    Solucao solucaoGulosa(rotas, terceirizacao, custoTotal, custoTerceirizacaoSolucao, custoVeiculosSolucao, custoRotasSolucao);
    return solucaoGulosa;
}
//função que calcula o melhor swap intra rota
t_swap melhorSwapIntraRota(vector<vector<int>> rotas, vector<vector<int>> custos, int nEntregas){
    
    //m_v = melhor veiculo
    //m_i = index de entrega para se realizada o swap (i ----Swap---> para j)
    //m_j = index com qual se realiza o swap
    //m_custo = melhor custo
    //custo = custo do swap atual (negativo acontece uma melhora no custo), (positivo acontece uma piora no custo), (0 não acontece nada no custo)

    int m_v = 0, m_i = 0, m_j = 0, m_custo = 9999999, custo;

    t_swap melhorSwap;

    for (int v = 0; v < int(rotas.size()); v++){
        
        vector<int> rota = rotas[v];
        int tam_rota = int(rota.size());

        if(tam_rota > 3){
            for (int i = 1; i < tam_rota-2; i++){
                for (int j = i+1; j < tam_rota-1 ; j++){
                    if (i == j-1){
                        custo = custos[rota[i-1]][rota[j]] + custos[rota[j]][rota[i]] + custos[rota[i]][rota[j+1]] - custos[rota[i-1]][rota[i]] - custos[rota[i]][rota[j]] - custos[rota[j]][rota[j+1]];
                    }else{
                        custo = custos[rota[i-1]][rota[j]] + custos[rota[j]][rota[i+1]] + custos[rota[j-1]][rota[i]] + custos[rota[i]][rota[j+1]] - custos[rota[i-1]][rota[i]] - custos[rota[i]][rota[i+1]] - custos[rota[j-1]][rota[j]] - custos[rota[j]][rota[j+1]];
                    }
                    if(custo < m_custo){
                        m_v = v;
                        m_i = i;
                        m_j = j;
                        m_custo = custo;
                    }
                } 
            }
        }
    }

    melhorSwap.m_v = m_v;
    melhorSwap.m_i = m_i;
    melhorSwap.m_j = m_j;
    melhorSwap.m_custo = m_custo;

    return melhorSwap;
}
// função que calcula o melhor swap entre rotas
x_swap melhorSwapInterRotas(vector<vector<int>> rotas, vector<vector<int>> custos, vector<int> demandas, int capacidadeVeiculo){
    int m_v_a = 0, m_v_b = 0, m_i = 0,m_j = 0, m_custo = 9999999, custo;
    x_swap melhorSwap;
    // calcula a capacidade de cada rota
    vector<int> capacidadeDasRotas = calculaCapacidadeRota(demandas, rotas);
    // Para cada par de rotas checa se o swap entre elas é melhor que o melhor swap atual
    for (int v_a = 0; v_a < int(rotas.size()-1); v_a++){
        for (int v_b = v_a + 1; v_b < int(rotas.size()); v_b++){

            vector<int> rota_a = rotas[v_a];
            vector<int> rota_b = rotas[v_b];
            int tam_rota_a = int(rota_a.size());
            int tam_rota_b = int(rota_b.size());
            for (int i = 1; i < tam_rota_a-1; i++){
                for (int j = 1; j < tam_rota_b-1; j++){
                    //verifica se a troca respeita a capacidade das rotas
                    if (capacidadeDasRotas[v_a] - demandas[rota_a[i] - 1] + demandas[rota_b[j] - 1] <= capacidadeVeiculo && capacidadeDasRotas[v_b] - demandas[rota_b[j] - 1] + demandas[rota_a[i] - 1] <= capacidadeVeiculo){

                        custo = custos[rota_a[i-1]][rota_b[j]] + custos[rota_b[j]][rota_a[i+1]] + custos[rota_b[j-1]][rota_a[i]] + custos[rota_a[i]][rota_b[j+1]] - custos[rota_a[i-1]][rota_a[i]] - custos[rota_a[i]][rota_a[i+1]] - custos[rota_b[j-1]][rota_b[j]] - custos[rota_b[j]][rota_b[j+1]];

                        if(custo < m_custo){
                            m_v_a = v_a;
                            m_v_b = v_b;
                            m_i = i;
                            m_j = j;
                            m_custo = custo;
                        }
                    }
                }
            }
        }  
    }

    melhorSwap.m_v_a = m_v_a;
    melhorSwap.m_v_b = m_v_b;
    melhorSwap.m_i = m_i;
    melhorSwap.m_j = m_j;
    melhorSwap.m_custo = m_custo;

    return melhorSwap;
}
// função que calcula a melhor terceirização de uma entrega
i_terceirizado melhorTerceirizacao(vector<vector<int>> rotas, vector<vector<int>> custos, vector<int> demandas, vector<int> terceirizados, vector<int> custoTerceirizacao, int capacidadeVeiculo, int minEntrega, int custoVeiculo){
    int m_v = 0, m_i = 0, m_custo_rotas = 9999999, m_custo_ter = 9999999, m_custo_veiculo = 0;

    i_terceirizado melhorTerceirizacao;
    // Calcula a quantidade de entregas de cada rota
    int quantidadeEntregas = calculaQuantidadeEntregas(rotas);
    // Para cada entrega checa se o custo de terceirização dela é menor que o melhor custo de terceirização testada
    for (int v = 0; v < int(rotas.size()); v++){
        vector<int> rota = rotas[v];
        int tam_rota = int(rota.size());
        for (int i = 1; i < tam_rota - 1; i++){\
            // Checa se atende a demanda minima de entregas
            if ( quantidadeEntregas - 1 > minEntrega){
                int custo_rotas = 0, custo_ter = 0, custo_veiculo = 0;

                if (tam_rota > 3) {
                    custo_rotas = custos[rota[i-1]][rota[i+1]] - custos[rota[i-1]][rota[i]] - custos[rota[i]][rota[i+1]];
                }else if ( tam_rota == 3 ) {
                    custo_rotas = - custos[rota[i-1]][rota[i]] - custos[rota[i]][rota[i+1]];
                    custo_veiculo -= custoVeiculo;
                }

                custo_ter = custoTerceirizacao[rota[i]-1];
                if (custo_rotas + custo_ter + custo_veiculo < m_custo_rotas + m_custo_ter + m_custo_veiculo){
                    m_v = v;
                    m_i = i;
                    m_custo_rotas = custo_rotas;
                    m_custo_ter = custo_ter;
                    m_custo_veiculo = custo_veiculo;
                }
            }
        }
    }

    melhorTerceirizacao.m_v = m_v;
    melhorTerceirizacao.m_i = m_i;
    melhorTerceirizacao.m_custo_rotas = m_custo_rotas;
    melhorTerceirizacao.m_custo_ter = m_custo_ter;
    melhorTerceirizacao.m_custo_veiculo = m_custo_veiculo;
    
    return melhorTerceirizacao;

}
//função que calcula o melhor insert de um terceirizado em uma rotas
t_terceirizado melhorInsertTerceirizado(vector<vector<int>> rotas, vector<vector<int>> custos, vector<int> demandas, vector<int> terceirizados, vector<int> custoTerceirizacao, int capacidadeVeiculo, int nEntregas, int custoVeiculo){
    int m_v = 0, m_i = 0, m_ter = 0, m_custo_rotas = 9999999, m_custo_ter = 9999999, m_custo_veiculo = 0;

    t_terceirizado melhorInsert;
    // Calcula a capacidade de cada rota
    vector<int> capacidadeDasRotas = calculaCapacidadeRota(demandas, rotas);
    // Para cada terceirizado checa se o custo de inserção dele em cada rota é menor que o melhor custo de inserção do terceirizado
    for ( int t = 0; t < int(terceirizados.size()); t++){
        for( int v = 0; v < int(rotas.size()); v++){
            vector<int> rota = rotas[v];
            int tam_rota = int(rota.size());
            for (int i = 1; i < tam_rota; i++){
                // Checa se atende a demanda minima de entrega
                if(capacidadeDasRotas[v] + demandas[terceirizados[t]-1] <= capacidadeVeiculo){
                    int custo_rotas, custo_ter, custo_veiculo = 0;
                    if(tam_rota > 2){
                        custo_rotas = custos[rota[i-1]][terceirizados[t]] + custos[terceirizados[t]][rota[i]] - custos[rota[i-1]][rota[i]];
                    }else{
                        custo_rotas = custos[rota[i-1]][terceirizados[t]] + custos[terceirizados[t]][rota[i]] - custos[rota[i-1]][rota[i]];
                        custo_veiculo += custoVeiculo;
                    }

                    custo_ter = - custoTerceirizacao[terceirizados[t]-1] ;
                    if(custo_rotas + custo_ter + custo_veiculo < m_custo_rotas + m_custo_ter + m_custo_veiculo){
                        m_v = v;
                        m_i = i;
                        m_ter = t;
                        m_custo_rotas = custo_rotas;
                        m_custo_ter = custo_ter;
                        m_custo_veiculo = custo_veiculo;
                    }
                }
                
            }
        }
    }

    melhorInsert.m_v = m_v;
    melhorInsert.m_i = m_i;
    melhorInsert.m_ter = m_ter;
    melhorInsert.m_custo_rotas = m_custo_rotas;
    melhorInsert.m_custo_ter = m_custo_ter;
    melhorInsert.m_custo_veiculo = m_custo_veiculo;

    return melhorInsert;

}
//Implementa a busca local VND
Solucao VND(Solucao solucaoAtual, int nVizinhancas, vector<vector<int>> custos, int nEntregas, int minEntrega, vector<int> demandas , vector<int> custoTerceirizacao, int capacidadeVeiculo, int custoVeiculo){
    int k = 1;
    Solucao solucaoVizinha = solucaoAtual;
    vector<int> terceirizados = solucaoVizinha.terceirizados;
    t_swap melhorSwap;
    x_swap melhorSwapRotas;
    t_terceirizado melhorInsert;
    i_terceirizado melhorTer;

    while (k <= nVizinhancas){
        switch (k){
        case 1: //Vizinhaça 1: Swap Intra Rota
            melhorSwap = melhorSwapIntraRota(solucaoVizinha.rotas, custos, nEntregas);
            if (melhorSwap.m_custo < 0){
                // Faz o swap intra rota
                int aux = solucaoVizinha.rotas[melhorSwap.m_v][melhorSwap.m_i];
                solucaoVizinha.rotas[melhorSwap.m_v][melhorSwap.m_i] = solucaoVizinha.rotas[melhorSwap.m_v][melhorSwap.m_j];
                solucaoVizinha.rotas[melhorSwap.m_v][melhorSwap.m_j] = aux;
                solucaoVizinha.custoRotas += melhorSwap.m_custo;
                solucaoVizinha.custoTotal += melhorSwap.m_custo;
                k = 1;
            }else{
                k++;
            }
            break;
        case 2: //Vizinhaça 2: Swap entre rotas
            melhorSwapRotas = melhorSwapInterRotas(solucaoVizinha.rotas, custos, demandas, capacidadeVeiculo);
            if (melhorSwapRotas.m_custo < 0){
                // Faz o swap entre rotas
                int aux = solucaoVizinha.rotas[melhorSwapRotas.m_v_a][melhorSwapRotas.m_i];
                solucaoVizinha.rotas[melhorSwapRotas.m_v_a][melhorSwapRotas.m_i] = solucaoVizinha.rotas[melhorSwapRotas.m_v_b][melhorSwapRotas.m_j];
                solucaoVizinha.rotas[melhorSwapRotas.m_v_b][melhorSwapRotas.m_j] = aux;
                solucaoVizinha.custoRotas += melhorSwapRotas.m_custo;
                solucaoVizinha.custoTotal += melhorSwapRotas.m_custo;
                k = 1;
            }else{
                k++;
            }
            break;
        case 3: //Vizinhaça 3: Insert de um terceirizado em uma rota
            melhorInsert = melhorInsertTerceirizado(solucaoVizinha.rotas, custos, demandas, solucaoVizinha.terceirizados, custoTerceirizacao, capacidadeVeiculo, nEntregas, custoVeiculo);
            if (melhorInsert.m_custo_rotas + melhorInsert.m_custo_ter + melhorInsert.m_custo_veiculo < 0){
                //Faz a melhor inserção de terceirizado
                solucaoVizinha.rotas[melhorInsert.m_v].insert(solucaoVizinha.rotas[melhorInsert.m_v].begin()+melhorInsert.m_i, solucaoVizinha.terceirizados[melhorInsert.m_ter]);
                solucaoVizinha.terceirizados.erase(solucaoVizinha.terceirizados.begin()+melhorInsert.m_ter);
                solucaoVizinha.custoRotas += melhorInsert.m_custo_rotas;
                solucaoVizinha.custoTerceirizacao += melhorInsert.m_custo_ter;
                solucaoVizinha.custoVeiculo += melhorInsert.m_custo_veiculo;
                solucaoVizinha.custoTotal += melhorInsert.m_custo_rotas + melhorInsert.m_custo_ter + melhorInsert.m_custo_veiculo;
                k = 1;
            }else{
                k++;
            }
            break;
        case 4: //Vizinhaça 4: Terceirização de uma entrega
            melhorTer = melhorTerceirizacao(solucaoVizinha.rotas, custos, demandas, solucaoVizinha.terceirizados, custoTerceirizacao, capacidadeVeiculo, minEntrega, custoVeiculo);
            if (melhorTer.m_custo_rotas + melhorTer.m_custo_ter + melhorTer.m_custo_veiculo < 0){
                //Faz a melhor terceirização
                solucaoVizinha.terceirizados.push_back(solucaoVizinha.rotas[melhorTer.m_v][melhorTer.m_i]);
                solucaoVizinha.rotas[melhorTer.m_v].erase(solucaoVizinha.rotas[melhorTer.m_v].begin()+melhorTer.m_i);
                solucaoVizinha.custoRotas += melhorTer.m_custo_rotas;
                solucaoVizinha.custoTerceirizacao += melhorTer.m_custo_ter;
                solucaoVizinha.custoVeiculo += melhorTer.m_custo_veiculo;
                solucaoVizinha.custoTotal += melhorTer.m_custo_rotas + melhorTer.m_custo_ter + melhorTer.m_custo_veiculo;
                k = 1;
            }else{
                k++;
            }
            break;
        default:

            break;

        }
    }
    return solucaoVizinha;
}
// Implementa a perturbação do ILS
Solucao perturbacao(Solucao solAtual, vector<vector<int>> custos){
    Solucao solucaoPerturbada = solAtual;
    int num;
    // Faz uma perturbaçao em cada rota da solucao
    for(int v = 0; v < int(solucaoPerturbada.rotas.size()); v++){
        vector<int> rota = solucaoPerturbada.rotas[v];
        rota.erase(rota.begin());
        rota.pop_back();
        int tam_rota = int(rota.size());
        // Checa o tamanho da rota
        if (tam_rota > 1){
            num = ceil((tam_rota/2));
            //Faz o shift, colocando os primeiros num elementos no final da rota
            for(int i = 0; i < num; i++){
                rota.push_back(rota[0]);
                rota.erase(rota.begin());
            }
            //Recalcula o custo da rota
            int deltaCustoRota = custos[solAtual.rotas[v][0]][solAtual.rotas[v][num+1]] + custos[solAtual.rotas[v][num]][solAtual.rotas[v][0]] + custos[solAtual.rotas[v][tam_rota+2-2]][solAtual.rotas[v][1]] - custos[solAtual.rotas[v][0]][solAtual.rotas[v][1]] - custos[solAtual.rotas[v][tam_rota+2-2]][solAtual.rotas[v][0]] - custos[solAtual.rotas[v][num]][solAtual.rotas[v][num+1]];
            solucaoPerturbada.custoRotas += deltaCustoRota;
            solucaoPerturbada.custoTotal += deltaCustoRota;
        }
        rota.insert(rota.begin(), 0);
        rota.push_back(0);
        solucaoPerturbada.rotas[v] = rota;
    }
    return solucaoPerturbada;
}

int main(int argc, char *argv[]) {

    //Tempos de execucao:
    clock_t otimo_s;
    clock_t otimo_e;
    clock_t gulosa_s;
    clock_t gulosa_e;
    clock_t vizinho_s;
    clock_t vizinho_e;

    // Verifique se o número de argumentos está correto
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <arquivo_de_instancia> <valor_solucao_otima>" << endl;
        return 1;
    }

    // Leitura da instância
    string arquivoDeEntrada = argv[1];
    float valorSolucaoOtima = atof(argv[2]);
    Instancia instancia(arquivoDeEntrada);

    
    // ILS
    // Define o numero de iteracoes do ILS
    int iterILS;
    if (instancia.nEntregas < 100){
        iterILS = 50;
    }else{
        iterILS = int(instancia.nEntregas/2);
    }
    //Faz a solucao gulosa e contabiliza o tempo
    gulosa_s = clock();
    Solucao solucaoGulosa = guloso(instancia.nVeiculos, instancia.nEntregas, instancia.capacidadeVeiculo, instancia.minEntregas, instancia.custoVeiculo, instancia.demandas, instancia.custo, instancia.custosTerceirizacao);
    gulosa_e = clock();
    //Faz a solucao vizinha e contabiliza o tempo
    vizinho_s = clock();
    Solucao solucaoVizinha = VND(solucaoGulosa, 4, instancia.custo, instancia.nEntregas, instancia.minEntregas, instancia.demandas, instancia.custosTerceirizacao, instancia.capacidadeVeiculo, instancia.custoVeiculo);
    vizinho_e = clock();
    //Faz a solucao otima e contabiliza o tempo
    otimo_s = clock();
    Solucao solucaoOtima = solucaoVizinha;
    while (iterILS)
    {
        // Perturba a solucao
        Solucao solucaoPerturbada = perturbacao(solucaoOtima, instancia.custo);
        // Faz a busca local na solucao perturbada
        solucaoVizinha = VND(solucaoPerturbada, 4, instancia.custo, instancia.nEntregas, instancia.minEntregas, instancia.demandas, instancia.custosTerceirizacao, instancia.capacidadeVeiculo, instancia.custoVeiculo);
        if (solucaoVizinha.custoTotal < solucaoOtima.custoTotal){
            // Atualiza a solucao otima se houver melhora
            solucaoOtima = solucaoVizinha;
        }
        iterILS--;
    }
    otimo_e = clock();

    //Calculo do gap
    float gap_otima = ((solucaoOtima.custoTotal - valorSolucaoOtima)/valorSolucaoOtima)*100;
    float gap_gulosa = ((solucaoGulosa.custoTotal - valorSolucaoOtima)/valorSolucaoOtima)*100;
    float gap_vizinha = ((solucaoVizinha.custoTotal - valorSolucaoOtima)/valorSolucaoOtima)*100;

    //Tratamento das strings
    string nomeArquivo = arquivoDeEntrada.substr(arquivoDeEntrada.find_last_of('/') + 1); // Remove o caminho do arquivo de entrada
    string nomeinstancia = nomeArquivo.substr(0, nomeArquivo.find_last_of('.'));

    cout << "\nNome da instancia: " << nomeinstancia << endl;
    cout << "gap_gulosa: " << gap_gulosa << " gap_vizinha: " << gap_vizinha << " gap_otima: " << gap_otima << endl;
    cout << "custo gulosa: " << solucaoGulosa.custoTotal << " custo vizinho: " << solucaoVizinha.custoTotal << " custo otimo: " << solucaoOtima.custoTotal << endl;
    cout << "tempo gulosa: " << (gulosa_e - gulosa_s)/double(CLOCKS_PER_SEC) << " tempo vizinho: " << (vizinho_e - vizinho_s)/double(CLOCKS_PER_SEC) << " tempo otimo: " << (otimo_e - otimo_s)/double(CLOCKS_PER_SEC) << endl;

    //Print formato da tabela no Latex:
    //cout << nomeinstancia << " & " <<valorSolucaoOtima << " & " <<solucaoGulosa.custoTotal << " & " << (gulosa_e - gulosa_s)/double(CLOCKS_PER_SEC) << " & " << gap_gulosa<< " & " << solucaoVizinha.custoTotal<< " & " << (vizinho_e - vizinho_s)/double(CLOCKS_PER_SEC)<< " & "<< gap_vizinha<< " & " << solucaoOtima.custoTotal<< " & "<<( otimo_e - otimo_s)/double(CLOCKS_PER_SEC)<< " & " << gap_otima << "\\\\" << endl;

    string ArquivoSaida = "instanciasSolucoes/" + nomeArquivo.substr(0, nomeArquivo.find_last_of('.')) + "_solucao.txt"; // Remove a extensão do arquivo de entrada e adiciona a extensão .solucao.txt
    ofstream arquivoSaida(ArquivoSaida); // Crie o arquivo de saída.

    // Verifique se o arquivo foi criado com sucesso.
    if (!arquivoSaida){
        cerr << "Erro ao criar o arquivo de saída." << endl;
        return 1;
    }

    salvarSolucaoEmArquivo(solucaoOtima, arquivoSaida);
    arquivoSaida.close();



    return 0;
}
