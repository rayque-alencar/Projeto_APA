#include "Instancia.hpp"

// Construtor da classe Instancia
Instancia::Instancia(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);

    // Verificando se o arquivo foi aberto
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo." << endl; 
        exit(1); 
    }

    // Lendo os dados da instancia do arquivo
    arquivo >> nEntregas;
    arquivo >> nVeiculos;
    arquivo >> capacidadeVeiculo;
    arquivo >> minEntregas;
    arquivo >> custoVeiculo;

    // Lendo os dados do arquivo da demanda
    demandas.resize(nEntregas);                   
    for (int i = 0; i < nEntregas; i++) {
        arquivo >> demandas[i];
    }

    // Lendo os dados do arquivo do custo de terceirizacao
    custosTerceirizacao.resize(nEntregas);
    for (int i = 0; i < nEntregas; i++) {
        arquivo >> custosTerceirizacao[i];
    }

    // Lendo os dados do arquivo da matriz de custo
    custo.resize(nEntregas + 1, vector<int>(nEntregas + 1));
    for (int i = 0; i < nEntregas + 1; i++) {
        for (int j = 0; j < nEntregas + 1; j++) {
            arquivo >> custo[i][j];
        }
    }

    // Fechando o arquivo
    arquivo.close();
}

// Imprimindo os dados da instancia na tela
void Instancia::imprimirDados() {
    cout << "n: " << nEntregas << endl;
    cout << "k: " << nVeiculos << endl;
    cout << "Q: " << capacidadeVeiculo << endl;
    cout << "L: " << minEntregas << endl;
    cout << "r: " << custoVeiculo << endl;
    cout << "--------------------------" << endl;

    cout << "Array d:";
    for (int i = 0; i < nEntregas; i++) {
        cout << " " << demandas[i];
    }
    cout << "\n--------------------------" << endl;

    cout << "Array p:";
    for (int i = 0; i < nEntregas; i++) {
        cout << " " << custosTerceirizacao[i];
    }
    cout << "\n--------------------------" << endl;

    cout << "Matriz c:" << endl;
    for (int i = 0; i < nEntregas + 1; i++) {
        for (int j = 0; j < nEntregas + 1; j++) {
            cout << custo[i][j] << " ";
        }
        cout << endl;
    }
    cout << "--------------------------" << endl;
}
