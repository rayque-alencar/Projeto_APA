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


    rotas.resize(num_De_Veiculos); // Crie o vetor de rotas com o tamanho certo.
    entregasAlocadas.resize(num_de_Entregas, false);

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

int Instancia::calcularCustoRota(const vector<int>& rota) {
    int custoRota = 0;
    for (int i = 0; i < rota.size() - 1; i++) {
        int entrega1 = rota[i];
        int entrega2 = rota[i + 1];
        custoRota += custo[entrega1][entrega2];
    }
    
    // Adicione o custo de retorno ao depósito (do último vértice de volta ao 0).
    int ultimaEntrega = rota.back();
    custoRota += custo[ultimaEntrega][0]; // 0 representa o depósito.

    return custoRota;
}

int Instancia::calcularCustoTerceirizacao(int entrega) {
    int custoTerceirizacao = custos_De_Terceirizacao[entrega];

    // Calcule o custo das entregas não atendidas se a entrega for terceirizada
    for (int i = 0; i < num_de_Entregas; i++) {
        if (!entregasAlocadas[i]) {
            custoTerceirizacao += custos_De_Terceirizacao[i];
        }
    }

    return custoTerceirizacao;
}

void Instancia::algoritmoGuloso() {
    this->rotas.resize(num_De_Veiculos);
    vector<bool> entregasAlocadas(num_de_Entregas, false);

    int entregasNaoTerceirizadas = 0; // Contador para entregas não terceirizadas.

    for (int veiculo = 0; veiculo < num_De_Veiculos; veiculo++) {
        int capacidadeRestante = capacidade_Veiculo;
        vector<int> rota;
        rota.push_back(0); // Começa no local 0
        int custoRota = 0;
        int pesoRota = 0;

        while (true) {
            int melhorEntrega = -1;
            int custoMelhorEntrega = INT_MAX;

            for (int entrega = 1; entrega <= num_de_Entregas; entrega++) {
                if (!entregasAlocadas[entrega] && pesoRota + demadas[entrega] <= capacidadeRestante) {
                    vector<int> novaRota = rota;
                    novaRota.push_back(entrega);
                    int custoEntrega = calcularCustoRota(novaRota);

                    if (veiculo >= min_De_Entregas) {
                        int custoTerceirizacao = calcularCustoTerceirizacao(entrega);
                        if (custoTerceirizacao < custoMelhorEntrega) {
                            custoMelhorEntrega = custoTerceirizacao;
                            melhorEntrega = entrega;
                        }
                    }

                    if (custoEntrega < custoMelhorEntrega) {
                        custoMelhorEntrega = custoEntrega;
                        melhorEntrega = entrega;
                    }
                }
            }

            if (melhorEntrega == -1) {
                break;
            }

            if (custoMelhorEntrega == INT_MAX) {
                // Se o custoMelhorEntrega ainda for INT_MAX, não encontramos nenhuma entrega adequada,
                // então terminamos a rota.
                break;
            }

            // Adicione a entrega à rota do veículo
            rota.push_back(melhorEntrega);
            pesoRota += demadas[melhorEntrega];
            capacidadeRestante -= demadas[melhorEntrega];
            custoRota = custoMelhorEntrega;
            entregasAlocadas[melhorEntrega] = true;

            if (custoMelhorEntrega == calcularCustoTerceirizacao(melhorEntrega)) {
                entregasNaoTerceirizadas++; // A entrega não foi terceirizada.
            }

            if (entregasNaoTerceirizadas >= min_De_Entregas) {
                // Alcançou o mínimo de entregas não terceirizadas.
                break;
            }
        }

        // Adicione o retorno para o local 0 ao final de cada rota
        rota.push_back(0);

        this->rotas[veiculo] = rota;

        cout << "Rota do Veículo " << veiculo + 1 << ": ";
        for (int i = 0; i < rota.size(); i++) {
            cout << rota[i] << " "; // Imprime as entregas alocadas na rota.
        }

        cout << "Custo da Rota: " << custoRota << endl;
    }
}




void Instancia::imprimirRotas() {
    cout << "quantidade veiculos: " << rotas.size() << endl;
    for (size_t veiculo = 0; veiculo < rotas.size(); veiculo++) {
        cout << "Rota do Veículo " << veiculo + 1 << ": ";
        cout << "tamanho rotas: " << this->rotas[veiculo].size() << endl;
        for (int entrega : this->rotas[veiculo]) {
            cout << entrega << " ";
        }
        cout << endl;
    }
}




// // Função para realizar movimentos envolvendo uma única rota
// void Vizinhanca1(Instancia& instancia) {
//     for (int veiculo = 0; veiculo < instancia.num_De_Veiculos; veiculo++) {
//         vector<int> rota = instancia.rotas[veiculo];
        
//         for (int i = 0; i < rota.size(); i++) {
//             for (int j = i + 1; j < rota.size(); j++) {
//                 // Troca a ordem das entregas i e j na rota
//                 swap(rota[i], rota[j]);
                
//                 // Verifica se a capacidade da rota ainda é respeitada
//                 if (VerificarCapacidade(rota, instancia)) {
//                     // Atualize a solução se a nova rota for melhor
//                     AtualizarSolucao(instancia, veiculo, rota);
//                 }
//                 else {
//                     // Desfaz a troca para tentar outras combinações
//                     swap(rota[i], rota[j]);
//                 }
//             }
//         }
//     }
// }
