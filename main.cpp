#include "Instancia.hpp"

void guloso(int nVeiculos, int nEntregas, int capacidadeVeiculo, int minEntregas, vector<int> demandas, vector<vector<int>> custo, vector<int> custosTerceirizacao){
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
                int custoArco = custo[rotas[v].back()][candidatos[i]];

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
        cout << "--------------------------" << endl;
        cout << "Melhor candidato: " << melhorCandidato << endl;
        cout << "Melhor custo: " << melhorCusto << endl;
        cout << "Melhor veiculo: " << melhorV << endl;
        cout << "Peso: " << pesos[melhorV] << endl;
        cout << "Retirei: " << candidatos[iMelhorCandidato] << endl;
        candidatos.erase(candidatos.begin()+iMelhorCandidato);

        for (int i = 0; i < nVeiculos; i++)
        {
            cout << "Rota " << i << ": ";
            for (int j = 0; j < int(rotas[i].size()); j++)
            {
                cout << rotas[i][j] << " ";
            }
            cout << endl;
        }

        cout << "Terceirizacao: ";
        for (int i = 0; i < int(terceirizacao.size()); i++)
        {
            cout << terceirizacao[i] << " ";
        }
        cout << endl;
            
        }
        cout << "--------------------------" << endl;
    
}
    

int main() {
    
    string nomeArquivo = "instancias/n9k5_D.txt";
    Instancia instancia(nomeArquivo);

    instancia.imprimirDados();

    //instancia.guloso();

    guloso(instancia.nVeiculos, instancia.nEntregas, instancia.capacidadeVeiculo, instancia.minEntregas, instancia.demandas, instancia.custo, instancia.custosTerceirizacao);

    return 0;
}
