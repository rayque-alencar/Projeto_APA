#include "Instancia.hpp"
#include "Solucao.hpp"

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

}t_terceirizado;

//funçãao que calcula o custo das rotas
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

        
                /*if (rotas[v].back() == 0){
                    custoArco = custo[rotas[v].back()][candidatos[i]]+custoVeiculo;
                }else{
                    custoArco = custo[rotas[v].back()][candidatos[i]];
                }*/

                custoArco = custo[rotas[v].back()][candidatos[i]];

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
        
        // cout << "--------------------------" << endl;
        // cout << "Melhor candidato: " << melhorCandidato << endl;
        // cout << "Melhor custo: " << melhorCusto << endl;
        // cout << "Melhor veiculo: " << melhorV << endl;
        // cout << "Peso: " << pesos[melhorV] << endl;
        // cout << "Retirei: " << candidatos[iMelhorCandidato] << endl;

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

//função que calcula a capacidade de cada rota
vector<int> calculaCapacidadeRota(vector<int> demandas, vector<vector<int>> rotas){
    vector<int> capacidadeRota(rotas.size(), 0);
    for (int i = 0; i < int(rotas.size()); i++){
        for (int j = 0; j < int(rotas[i].size()); j++){
            capacidadeRota[i] += demandas[rotas[i][j]-1];
        }
        //cout << "capacidadeRota: " << capacidadeRota[i] << endl;
    }
    return capacidadeRota;
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

    //cout << "O melhor swap é entre: " << rotas[m_v][m_i] << " e " << rotas[m_v][m_j] << "com custo: " << m_custo << endl;
    return melhorSwap;
}

x_swap melhorSwapInterRotas(vector<vector<int>> rotas, vector<vector<int>> custos, vector<int> demandas, int capacidadeVeiculo){
    int m_v_a = 0, m_v_b = 0, m_i = 0,m_j = 0, m_custo = 9999999, custo;

    //cout << "Entrou no swap inter rotas" << endl;
    x_swap melhorSwap;

    vector<int> capacidadeDasRotas = calculaCapacidadeRota(demandas, rotas);

    for (int v_a = 0; v_a < int(rotas.size()-1); v_a++){
        for (int v_b = v_a + 1; v_b < int(rotas.size()); v_b++){

            vector<int> rota_a = rotas[v_a];
            vector<int> rota_b = rotas[v_b];
            int tam_rota_a = int(rota_a.size());
            int tam_rota_b = int(rota_b.size());
            for (int i = 1; i < tam_rota_a-1; i++){
                for (int j = 1; j < tam_rota_b-1; j++){
                    // cout << "Tentou trocar: " << rota_a[i]<<" com: " << rota_b[j]<< endl;
                    // cout << "------------------------------------------" << endl;
                    // ver se a troca respeita a capacidade das rotas
                    if (capacidadeDasRotas[v_a] - demandas[rota_a[i] - 1] + demandas[rota_b[j] - 1] <= capacidadeVeiculo && capacidadeDasRotas[v_b] - demandas[rota_b[j] - 1] + demandas[rota_a[i] - 1] <= capacidadeVeiculo){
                        // cout << "---------------dentro do if---------------------------" << endl;
                        // cout << "Demanda do cara a ser trocado: " << demandas[rota_a[i] - 1] << endl;
                        // cout << "Demanda do cara que vai entrar: " << demandas[rota_b[j] - 1] << endl;
                        // cout << "Tentou trocar: " << rota_a[i]<<" com: " << rota_b[j]<< endl;

                        custo = custos[rota_a[i-1]][rota_b[j]] + custos[rota_b[j]][rota_a[i+1]] + custos[rota_b[j-1]][rota_a[i]] + custos[rota_a[i]][rota_b[j+1]] - custos[rota_a[i-1]][rota_a[i]] - custos[rota_a[i]][rota_a[i+1]] - custos[rota_b[j-1]][rota_b[j]] - custos[rota_b[j]][rota_b[j+1]];

                        if(custo < m_custo){
                            m_v_a = v_a;
                            m_v_b = v_b;
                            m_i = i;
                            m_j = j;
                            m_custo = custo;
                            /*cout << "custo: " << custo << endl;
                            cout << "m_v_a: " << m_v_a << endl;
                            cout << "m_v_b: " << m_v_b << endl;
                            cout << "m_i: " << m_i << endl;
                            cout << "m_j: " << m_j << endl;
                            cout << "m_custo: " << m_custo << endl;
                            cout << "Swap é entre: " << rotas[m_v_a][m_i] << " e " << rotas[m_v_b][m_j] << endl;
                            cout << "------------------------------------------" << endl;*/
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

//função que calcula o melhor insert de um terceirizado em uma rotas
t_terceirizado melhorInsertTerceirizado(vector<vector<int>> rotas, vector<vector<int>> custos, vector<int> demandas, vector<int> terceirizados, vector<int> custoTerceirizacao, int capacidadeVeiculo, int nEntregas, int custoVeiculo){
    int m_v = 0, m_i = 0, m_ter = 0, m_custo_rotas = 9999999, m_custo_ter = 9999999, custo_rotas, custo_ter;

    t_terceirizado melhorInsert;

    vector<int> capacidadeDasRotas = calculaCapacidadeRota(demandas, rotas);

    for ( int t = 0; t < int(terceirizados.size()); t++){
        for( int v = 0; v < int(rotas.size()); v++){
            vector<int> rota = rotas[v];
            int tam_rota = int(rota.size());
            for (int i = 1; i < tam_rota; i++){
                if(capacidadeDasRotas[v] + demandas[terceirizados[t]-1] <= capacidadeVeiculo){
                    if(tam_rota > 2){
                        custo_rotas = custos[rota[i-1]][terceirizados[t]] + custos[terceirizados[t]][rota[i]] - custos[rota[i-1]][rota[i]];
                    }else{
                        custo_rotas = custos[rota[i-1]][terceirizados[t]] + custos[terceirizados[t]][rota[i]] - custos[rota[i-1]][rota[i]] + custoVeiculo;
                    }
                    //cout << "custo_rotasasdas: " << custo_rotas << endl;

                    custo_ter = - custoTerceirizacao[terceirizados[t]-1] ;
                    //cout << "Tentou inserir: " << terceirizados[t] <<" entre : " << rota[i-1] << " e " << rota[i] << endl;
                    if(custo_rotas + custo_ter < m_custo_rotas + m_custo_ter){
                        m_v = v;
                        m_i = i;
                        m_ter = t;
                        m_custo_rotas = custo_rotas;
                        m_custo_ter = custo_ter;
                        /*cout << "custo_rotas: " << m_custo_rotas << endl;
                        cout << "custo_ter: " << m_custo_ter << endl;
                        cout << "m_ter: " << m_ter << endl;
                        cout << "m_v: " << m_v << endl;
                        cout << "m_i: " << m_i << endl;*/
                        
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

    return melhorInsert;

}

Solucao VND(Solucao solucaoAtual, int nVizinhancas, vector<vector<int>> custos, int nEntregas, vector<int> demandas , vector<int> terceirizados, vector<int> custoTerceirizacao, int capacidadeVeiculo, int custoVeiculo){
    int k = 1;
    Solucao solucaoVizinha = solucaoAtual;
    t_swap melhorSwap;
    x_swap melhorSwapRotas;
    t_terceirizado melhorInsert;

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
        case 2: //Vizinhaça 2: Swap entre rotas
            melhorSwapRotas = melhorSwapInterRotas(solucaoVizinha.rotas, custos, demandas, capacidadeVeiculo);
            if (melhorSwapRotas.m_custo < 0){
                int aux = solucaoVizinha.rotas[melhorSwapRotas.m_v_a][melhorSwapRotas.m_i];
                solucaoVizinha.rotas[melhorSwapRotas.m_v_a][melhorSwapRotas.m_i] = solucaoAtual.rotas[melhorSwapRotas.m_v_b][melhorSwapRotas.m_j];
                solucaoVizinha.rotas[melhorSwapRotas.m_v_b][melhorSwapRotas.m_j] = aux;
                solucaoVizinha.custoRotas += melhorSwapRotas.m_custo;
                solucaoVizinha.custoTotal += melhorSwapRotas.m_custo;
                k = 1;
            }else{
                k++;
            }
            break;
        case 3:
            melhorInsert = melhorInsertTerceirizado(solucaoVizinha.rotas, custos, demandas, solucaoVizinha.terceirizados, custoTerceirizacao, capacidadeVeiculo, nEntregas, custoVeiculo);
            if (melhorInsert.m_custo_rotas + melhorInsert.m_custo_ter < 0){
                solucaoVizinha.rotas[melhorInsert.m_v].insert(solucaoVizinha.rotas[melhorInsert.m_v].begin()+melhorInsert.m_i, solucaoVizinha.terceirizados[melhorInsert.m_ter]);
                solucaoVizinha.terceirizados.erase(solucaoVizinha.terceirizados.begin()+melhorInsert.m_ter);
                solucaoVizinha.custoRotas += melhorInsert.m_custo_rotas;
                solucaoVizinha.custoTerceirizacao += melhorInsert.m_custo_ter;
                solucaoVizinha.custoTotal += melhorInsert.m_custo_rotas + melhorInsert.m_custo_ter;
                k = 1;
            }else{
                k++;
            }
        default:

            break;

        }
    }
    return solucaoVizinha;
}

Solucao perturbacao(Solucao solAtual, vector<int> terceirizados, vector<int> demandas, vector<int> custoTerceirizacao, int capacidadeVeiculo, int custoVeiculo){

}

int main() {
       
    string arquivoDeEntrada = "instancias/n64k9_A.txt";
    Instancia instancia(arquivoDeEntrada);

    //instancia.imprimirDados();

    Solucao solucaoGulosa = guloso(instancia.nVeiculos, instancia.nEntregas, instancia.capacidadeVeiculo, instancia.minEntregas, instancia.custoVeiculo, instancia.demandas, instancia.custo, instancia.custosTerceirizacao);
    
    cout << "#################GULOSO################" << endl;

    imprimirSolucao(solucaoGulosa);

    cout << "######################################" << endl;

    t_terceirizado melhorInsert;

    x_swap melhorSwap;

    // melhorSwap = melhorSwapInterRotas(solucaoGulosa.rotas, instancia.custo, instancia.demandas, instancia.capacidadeVeiculo, instancia.custoVeiculo);

    // cout << "######################################" << endl;
    // cout << "m_v_a: " << melhorSwap.m_v_a << endl;
    // cout << "m_v_b: " << melhorSwap.m_v_b << endl;
    // cout << "m_i: " << melhorSwap.m_i << endl;
    // cout << "m_j: " << melhorSwap.m_j << endl;
    // cout << "m_custo: " << melhorSwap.m_custo << endl;

    // melhorInsert = melhorInsertTerceirizado(solucaoGulosa.rotas, instancia.custo, instancia.demandas, solucaoGulosa.terceirizados, instancia.custosTerceirizacao, instancia.capacidadeVeiculo, instancia.nEntregas, instancia.custoVeiculo);

    // cout << "######################################" << endl;
    // cout << "m_v: " << melhorInsert.m_v << endl;
    // cout << "m_i: " << melhorInsert.m_i << endl;
    // cout << "m_ter: " << melhorInsert.m_ter << endl;
    // cout << "m_custo_rotas: " << melhorInsert.m_custo_rotas << endl;
    // cout << "m_custo_ter: " << melhorInsert.m_custo_ter << endl;

    //Solucao solucaoVizinha = VND(solucaoGulosa, 2, instancia.custo, instancia.nEntregas, instancia.demandas, instancia.capacidadeVeiculo, instancia.custoVeiculo, instancia.custosTerceirizacao);
    
    Solucao solucaoVizinha = VND(solucaoGulosa, 3, instancia.custo, instancia.nEntregas, instancia.demandas, solucaoGulosa.terceirizados, instancia.custosTerceirizacao, instancia.capacidadeVeiculo, instancia.custoVeiculo);

    cout << "################VIZINHO################" << endl;
    imprimirSolucao(solucaoVizinha);
    cout << "#######################################" << endl;


    /*t_swap melhorSwap;
    melhorSwap = melhorSwapIntraRota(solucaoGulosa.rotas, instancia.custo, instancia.nEntregas);
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
