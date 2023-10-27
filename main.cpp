#include "Instancia.hpp"
#include "Solucao.hpp"

//funçãao que calcula o custo das rotas
int custoRotas(vector<vector<int>> rotas, vector<vector<int>> custo){
    int custoRotas = 0;
    for(vector<int> rota : rotas){
        for (int i = 0; i < int(rota.size())-1; i++)
        {
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
        
        //cout << "--------------------------" << endl;
        //cout << "Melhor candidato: " << melhorCandidato << endl;
        //cout << "Melhor custo: " << melhorCusto << endl;
        //cout << "Melhor veiculo: " << melhorV << endl;
        //cout << "Peso: " << pesos[melhorV] << endl;
        //cout << "Retirei: " << candidatos[iMelhorCandidato] << endl;

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

void imprimirSolucaoEmArquivo(const Solucao& solucao, std::ofstream& arquivo) {
    
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


int main() {
    
    string arquivoDeEntrada = "instancias/n9k5_A.txt";
    Instancia instancia(arquivoDeEntrada);

    instancia.imprimirDados();

    Solucao solucaoGulosa = guloso(instancia.nVeiculos, instancia.nEntregas, instancia.capacidadeVeiculo, instancia.minEntregas, instancia.custoVeiculo, instancia.demandas, instancia.custo, instancia.custosTerceirizacao);
    
    imprimirSolucao(solucaoGulosa);
  
    string ArquivoSaida = arquivoDeEntrada.substr(0, arquivoDeEntrada.find_last_of('.')) + "_solucao.txt"; // Nome do arquivo de saída.
    ofstream arquivoSaida(ArquivoSaida); // Crie o arquivo de saída.

    // Verifique se o arquivo foi criado com sucesso.
    if (!arquivoSaida){
        cerr << "Erro ao criar o arquivo de saída." << endl;
        return 1;
    }

    // Chame a função para imprimir a solução no arquivo.
    imprimirSolucaoEmArquivo(solucaoGulosa, arquivoSaida);
    arquivoSaida.close();


    return 0;
}
