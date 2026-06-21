#include "InterfaceJogo.hpp"
#include <iostream>
#include <limits>
using namespace std;

void InterfaceJogo::exibirTexto (std::string texto){
    cout << texto << endl;
}

void InterfaceJogo::renderizarDivisor (){
    cout << "==============================" << endl;
}

void InterfaceJogo::limparTela (){
    for(int i = 0; i < 50; i++){
        cout << endl;
    }
}

int InterfaceJogo::solicitarEscolha (std::vector<std::string> opcoes){
int tamanho = static_cast<int>(opcoes.size());
    
    // Se não houver opções (programação defensiva), retorna um valor de salvaguarda
    if (tamanho == 0) return 0;

    int escolha = 0;

    while (true) {
        // 1. Exibe as opções na tela
        for (int i = 0; i < tamanho; i++) {
            std::cout << (i + 1) << " - " << opcoes[i] << std::endl;
        }
        std::cout << "Escolha uma opcao: ";

        // 2. Tenta ler a entrada do jogador
        if (std::cin >> escolha) {
            // Verifica se a escolha está dentro do intervalo válido
            if (escolha >= 1 && escolha <= tamanho) {
                return escolha; // Retorno correto e validado
            }
            std::cout << "\n[AVISO] Opcao invalida! Digite um numero entre 1 e " << tamanho << ".\n" << std::endl;
        } else {
            // 3. TRATAMENTO DE ERRO: Caso o usuário digite letras/caracteres inválidos
            std::cout << "\n[ERRO] Entrada invalida! Por favor, digite apenas numeros.\n" << std::endl;
            
            std::cin.clear(); // Limpa a flag de erro do cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta o texto incorreto do buffer
        }
    }
}

void InterfaceJogo::exibirStatus (const Personagem& p){
    cout << "Nome: " << p.getNome() << endl;
    cout << "HP: " << p.getHP() << "/" << p.getHPMax() << endl;
    cout << "Defesa: " << p.getDefesa() << endl;
    cout << "Nível: " << p.getNivel() << endl;
}

void InterfaceJogo::pausar(){
    std::cout << "\nPressione [ENTER] para continuar...";
    
    // Limpa qualquer caractere residual que tenha ficado no buffer do cin
    std::cin.clear();
    
    // Aguarda o usuário apertar Enter
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
