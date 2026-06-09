#include "InterfaceJogo.hpp"
#include <iostream>

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
   /*int size = static_cast<int>(opcoes.size());
    for(int i = 0; i < size; i++){
    cout << (i + 1) << " - " << opcoes[i] << endl;
    }
    int escolha = 0;
    cin >> escolha;
    if (escolha < 1 || escolha > size){
        return (3);
       }
    return (escolha);
    */
    return(1);
}

void InterfaceJogo::exibirStatus (const Personagem& p){
    cout << "Nome: " << p.getNome() << endl;
    cout << "HP: " << p.getHP() << "/" << p.getHPMax() << endl;
    cout << "Defesa: " << p.getDefesa() << endl;
    cout << "Nível: " << p.getNivel() << endl;
}

void InterfaceJogo::pausar(){
    
}
