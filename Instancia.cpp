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
    arquivo >> num_de_Entregas;
    arquivo >> num_De_Veiculos;
    arquivo >> capacidade_Veiculo;
    arquivo >> min_De_Entregas;
    arquivo >> custo_Do_Veiculo;

    // Lendo os dados do arquivo da demanda
    demadas.resize(num_de_Entregas);                   
    for (int i = 0; i < num_de_Entregas; i++) {
        arquivo >> demadas[i];
    }

    // Lendo os dados do arquivo do custo de terceirizacao
    custos_De_Terceirizacao.resize(num_de_Entregas);
    for (int i = 0; i < num_de_Entregas; i++) {
        arquivo >> custos_De_Terceirizacao[i];
    }

    // Lendo os dados do arquivo da matriz de custo
    custo.resize(num_de_Entregas + 1, vector<int>(num_de_Entregas + 1));
    for (int i = 0; i < num_de_Entregas + 1; i++) {
        for (int j = 0; j < num_de_Entregas + 1; j++) {
            arquivo >> custo[i][j];
        }
    }

    // Fechando o arquivo
    arquivo.close();
}

// Imprimindo os dados da instancia na tela
void Instancia::imprimirDados() {
    cout << "n: " << num_de_Entregas << endl;
    cout << "k: " << num_De_Veiculos << endl;
    cout << "Q: " << capacidade_Veiculo << endl;
    cout << "L: " << min_De_Entregas << endl;
    cout << "r: " << custo_Do_Veiculo << endl;
    cout << "--------------------------" << endl;

    cout << "Array d:";
    for (int i = 0; i < num_de_Entregas; i++) {
        cout << " " << demadas[i];
    }
    cout << "\n--------------------------" << endl;

    cout << "Array p:";
    for (int i = 0; i < num_de_Entregas; i++) {
        cout << " " << custos_De_Terceirizacao[i];
    }
    cout << "\n--------------------------" << endl;

    cout << "Matriz c:" << endl;
    for (int i = 0; i < num_de_Entregas + 1; i++) {
        for (int j = 0; j < num_de_Entregas + 1; j++) {
            cout << custo[i][j] << " ";
        }
        cout << endl;
    }
    cout << "--------------------------" << endl;
}