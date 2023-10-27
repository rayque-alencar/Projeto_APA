#include "Instancia.hpp"

int main() {
    
    string nomeArquivo = "instancias/n22k3_B.txt";
    Instancia instancia(nomeArquivo);

    instancia.algoritmoGuloso();
    //instancia.imprimirRotas();

    //instancia.imprimirDados();

    //printf("%d\n", instancia.custo[0][0]);

    return 0;
}
