#include "Instancia.hpp"
#include "Solucao.hpp"

//struct que guarda o melhor swap
typedef struct{
    int m_v = 0;
    int m_i = 0;
    int m_j = 0;
    int m_custo = 9999999;
}t_swap; 

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

t_swap melhorSwapIntraRota(vector<vector<int>> rotas, vector<vector<int>> custos, int nEntregas){
    
    //m_v = melhor veiculo
    //m_i = index de entrega para se realizada o swap (i ----Swap---> para j)
    //m_j = index com qual se realiza o swap
    //m_custo = melhor custo
    //custo = custo do swap atual (negativo acontece uma melhora no custo), (positivo acontece uma piora no custo), (0 não acontece nada no custo)

    int m_v = 0, m_i = 0, m_j = 0, m_custo = 9999999, custo;

    t_swap melhorSwap;

    for (int v = 0; v < int(rotas.size()); v++){
        
        //cout << "v: " << v << endl;
        vector<int> rota = rotas[v];
        int tam_rota = int(rota.size());

        if(tam_rota > 3){
            for (int i = 1; i < tam_rota-2; i++){
                for (int j = i+1; j < tam_rota-1 ; j++){
                    //cout << "Tentativas de SWAP entre: " << rota[i] << " e " << rota[j] << endl;
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
                        /*cout << "custo: " << custo << endl;
                        cout << "m_v: " << m_v << endl;
                        cout << "m_i: " << m_i << endl;
                        cout << "m_j: " << m_j << endl;
                        cout << "Swap é entre: " << rotas[m_v][m_i] << " e " << rotas[m_v][m_j] << endl;*/
                    }
                }
                
            }
            
        }
    }

    melhorSwap.m_v = m_v;
    melhorSwap.m_i = m_i;
    melhorSwap.m_j = m_j;
    melhorSwap.m_custo = m_custo;

    cout << "O melhor swap é entre: " << rotas[m_v][m_i] << " e " << rotas[m_v][m_j] << "com custo: " << m_custo << endl;
    return melhorSwap;
}

Solucao VND(Solucao solucaoAtual, int nVizinhancas, vector<vector<int>> custos, int nEntregas){
    int k = 1;
    Solucao solucaoVizinha = solucaoAtual;
    t_swap melhorSwap;

    while (k <= nVizinhancas){
        switch (k){
        case 1: //Vizinhaça 1: Swap Intra Rota
            melhorSwap = melhorSwapIntraRota(solucaoVizinha.rotas, custos, nEntregas);
            if (melhorSwap.m_custo < 0){
                int aux = solucaoVizinha.rotas[melhorSwap.m_v][melhorSwap.m_i];
                solucaoVizinha.rotas[melhorSwap.m_v][melhorSwap.m_i] = solucaoAtual.rotas[melhorSwap.m_v][melhorSwap.m_j];
                solucaoVizinha.rotas[melhorSwap.m_v][melhorSwap.m_j] = aux;
                solucaoVizinha.custoRotas += melhorSwap.m_custo;
                solucaoVizinha.custoTotal += melhorSwap.m_custo;
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

int main() {
    
    string arquivoDeEntrada = "instancias/arquivo_1.txt";
    Instancia instancia(arquivoDeEntrada);

    //instancia.imprimirDados();

    Solucao solucaoGulosa = guloso(instancia.nVeiculos, instancia.nEntregas, instancia.capacidadeVeiculo, instancia.minEntregas, instancia.custoVeiculo, instancia.demandas, instancia.custo, instancia.custosTerceirizacao);
    
    imprimirSolucao(solucaoGulosa);

    cout << "######################################" << endl;

    Solucao solucaoVizinha = VND(solucaoGulosa, 1, instancia.custo, instancia.nEntregas);

    imprimirSolucao(solucaoVizinha);

    /*t_swap melhorSwap;
    cout << "######################################" << endl;
    melhorSwap = melhorSwapIntraRota(solucaoGulosa.rotas, instancia.custo, instancia.nEntregas);
    cout << "######################################" << endl;
    cout << "m_v: " << melhorSwap.m_v << endl;
    cout << "m_i: " << melhorSwap.m_i << endl;
    cout << "m_j: " << melhorSwap.m_j << endl;
    cout << "m_custo: " << melhorSwap.m_custo << endl;*/

    string nomeArquivo = arquivoDeEntrada.substr(arquivoDeEntrada.find_last_of('/') + 1); // Remove o caminho do arquivo de entrada
    string ArquivoSaida = "instanciasSolucoes/" + nomeArquivo.substr(0, nomeArquivo.find_last_of('.')) + "_solucao.txt"; // Remove a extensão do arquivo de entrada e adiciona a extensão .solucao.txt
    ofstream arquivoSaida(ArquivoSaida); // Crie o arquivo de saída.

    // Verifique se o arquivo foi criado com sucesso.
    if (!arquivoSaida){
        cerr << "Erro ao criar o arquivo de saída." << endl;
        return 1;
    }

    salvarSolucaoEmArquivo(solucaoGulosa, arquivoSaida);
    arquivoSaida.close();

    return 0;
}
