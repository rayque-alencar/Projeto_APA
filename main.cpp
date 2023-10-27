#include "Instancia.hpp"

int main() {
    
    string nomeArquivo = "instancias/n120k7_A.txt";
    Instancia instancia(nomeArquivo);

    instancia.imprimirDados();

    return 0;
}
