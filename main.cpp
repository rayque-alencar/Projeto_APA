#include "Instancia.hpp"
#include "Solucao.hpp"


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

int custoTerceirizacao(vector<int> terceirizados, vector<int> custosTerceirizacao){
    int custoTerceirizacao = 0;
    for (int i = 0; i < int(terceirizados.size()); i++)
    {
        custoTerceirizacao += custosTerceirizacao[terceirizados[i]-1];
    }
    return custoTerceirizacao;
}

int custoVeiculos(int custoVeiculo, vector<vector<int>> rotas){
    int custoVeiculos = 0;
    for(vector<int> rota : rotas){
        if (rota[0] != rota[1])
        {
            custoVeiculos += custoVeiculo;
        }
    }
    return custoVeiculos;
}

Solucao guloso(int nVeiculos, int nEntregas, int capacidadeVeiculo, int minEntregas,int custoVeiculo, vector<int> demandas, vector<vector<int>> custo, vector<int> custosTerceirizacao){
    vector<vector<int>> rotas(nVeiculos);
    vector<int> terceirizacao;
    vector<int> pesos(nVeiculos, 0);
    vector<int> candidatos(nEntregas);

    for (int i = 1; i <= nEntregas; i++)
    {
        candidatos[i-1] = i;
    }

    for (int v = 0; v < nVeiculos; v++)
    {
        rotas[v].push_back(0);
        pesos.push_back(0);
    }
    
    while (!candidatos.empty())
    {
        int iMelhorCandidato = 0;
        int melhorV = 0;
        int melhorCusto = 999999999;
        for(int i = 0 ; i < int(candidatos.size()); i++){
            for (int v = 0; v < nVeiculos; v++)
            {
                int custoArco;

                if (rotas[v].back() == 0)
                {
                    custoArco = custo[rotas[v].back()][candidatos[i]]+custoVeiculo;
                }else{
                    custoArco = custo[rotas[v].back()][candidatos[i]];
                }

                if (custoArco < melhorCusto && pesos[v] + demandas[candidatos[i]-1] <= capacidadeVeiculo)
                {
                    iMelhorCandidato = i;
                    melhorCusto = custoArco;
                    melhorV = v;
                }
            }
        }

        int melhorCandidato = candidatos[iMelhorCandidato];

        if (nEntregas-int(candidatos.size()) > minEntregas && custosTerceirizacao[melhorCandidato] < melhorCusto)
        {
            terceirizacao.push_back(melhorCandidato);
        }
        else
        {
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

        //for (int i = 0; i < nVeiculos; i++)
        //{
        //    cout << "Rota " << i << ": ";
        //    for (int j = 0; j < int(rotas[i].size()); j++)
        //    {
        //        cout << rotas[i][j] << " ";
        //    }
        //    cout << endl;
        //}

        //cout << "Terceirizacao: ";
        //for (int i = 0; i < int(terceirizacao.size()); i++)
        //{
        //    cout << terceirizacao[i] << " ";
        //}
        //cout << endl;      
    }
    for (int v = 0; v < nVeiculos; v++)
    {
        rotas[v].push_back(0);
    }

    int custoRotasSolucao = custoRotas(rotas, custo);
    int custoTerceirizacaoSolucao = custoTerceirizacao(terceirizacao, custosTerceirizacao);
    int custoVeiculosSolucao = custoVeiculos(custoVeiculo, rotas);
    int custoTotal = custoRotasSolucao + custoTerceirizacaoSolucao + custoVeiculosSolucao;

    Solucao solucaoGulosa(rotas, terceirizacao, custoTotal, custoTerceirizacaoSolucao, custoVeiculosSolucao, custoRotasSolucao);
    cout << "--------------------------" << endl;
    for (int i = 0; i < nVeiculos; i++)
    {
        cout << "Rota " << i << ": ";
        for (int j = 0; j < int(rotas[i].size()); j++)
        {
            cout << rotas[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Custo das rotas: " << custoRotas(rotas, custo) << endl;
    cout << "Terceirizacao: ";
    for (int i = 0; i < int(terceirizacao.size()); i++)
    {
        cout << terceirizacao[i] << " ";
    }
    cout << endl;
    cout << "Custo da terceirizacao: " << custoTerceirizacao(terceirizacao, custosTerceirizacao) << endl;
    cout << "--------------------------" << endl;
    return solucaoGulosa;
}


int main() {
    
    string nomeArquivo = "instancias/arquivo_1.txt";
    Instancia instancia(nomeArquivo);

    instancia.imprimirDados();

    //instancia.guloso();
    Solucao solucaoGulosa = guloso(instancia.nVeiculos, instancia.nEntregas, instancia.capacidadeVeiculo, instancia.minEntregas, instancia.custoVeiculo, instancia.demandas, instancia.custo, instancia.custosTerceirizacao);
    //guloso(instancia.nVeiculos, instancia.nEntregas, instancia.capacidadeVeiculo, instancia.minEntregas, instancia.custoVeiculo, instancia.demandas, instancia.custo, instancia.custosTerceirizacao);
    cout << "Custo Total: " << solucaoGulosa.custoTotal << endl;
    cout << "Custo das rotas: " << solucaoGulosa.custoRotas << endl;
    cout << "Custo da terceirizacao: " << solucaoGulosa.custoTerceirizacao << endl;
    cout << "Custo dos veiculos: " << solucaoGulosa.custoVeiculo << endl;

    cout << "--------------------------" << endl;

    for (int i = 0; i < instancia.nVeiculos; i++)
    {
        cout << "Rota " << i << ": ";
        for (int j = 0; j < int(solucaoGulosa.rotas[i].size()); j++)
        {
            cout << solucaoGulosa.rotas[i][j] << " ";
        }
        cout << endl;
    }

    cout << "--------------------------" << endl;

    cout << "Terceirizacao: ";
    for (int i = 0; i < int(solucaoGulosa.terceirizados.size()); i++)
    {
        cout << solucaoGulosa.terceirizados[i] << " ";
    }
    cout << endl;

    cout << "--------------------------" << endl;

    return 0;
}
