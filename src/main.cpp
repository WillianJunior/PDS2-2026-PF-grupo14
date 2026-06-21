#include <iostream>
#include "Aventureiro.hpp"
#include "MotorJogo.hpp"
#include "InterfaceJogo.hpp"

int main() {
    try {
        // 1. Configurações Iniciais do Terminal (Opcional)
        // Se sua interface usar cores ou comandos específicos, configure aqui.
        InterfaceJogo::exibirTexto("==================================================");
        InterfaceJogo::exibirTexto("        INICIANDO AMBIENTE DE TESTE NA MAIN       ");
        InterfaceJogo::exibirTexto("==================================================");

        // 2. Criação do Protagonista
        // Instancie um herói com atributos controlados para testar o balanceamento
        Aventureiro heroi("Aragorn", 120, 10, 15);
        
        // 3. Inicialização do Motor do Jogo
        // Passa o herói para o Motor que gerenciará os estados e cenas
        MotorJogo motor(heroi);

        motor.rodar();

        
    } 
    catch (const std::exception& e) {
        // Captura qualquer erro inesperado ou lógica quebrada durante a gameplay
        std::cerr << "\n[ERRO CRÍTICO NA MAIN]: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}