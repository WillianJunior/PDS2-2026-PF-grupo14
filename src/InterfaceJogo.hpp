#ifndef INTERFACEJOGO_HPP
#define INTERFACEJOGO_HPP

#include "Personagem.hpp"
#include <string>
#include <vector>

/**
 * @file InterfaceJogo.hpp
 * @brief Definição da classe utilitária para interação com o usuário.
 */

/**
 * @class InterfaceJogo
 * @brief Classe puramente estática responsável pela entrada e saída de dados no console.
 * * Esta classe centraliza a formatação de textos, exibição de menus e leitura de escolhas,
 * garantindo que a lógica do jogo (backend) não dependa diretamente de comandos de I/O.
 */
class InterfaceJogo {
public:
    /**
     * @brief Exibe uma mensagem formatada para o jogador.
     * @param texto String contendo a mensagem ou descrição narrativa.
     */
    static void exibirTexto(std::string texto);

    /**
     * @brief Cria um menu numerado e valida a entrada do usuário.
     * * O método garante que o jogador escolha apenas opções válidas dentro do intervalo
     * do vetor fornecido.
     * * @param opcoes Vetor de strings com as descrições das opções (ex: "Atacar", "Fugir").
     * @return int O índice da opção escolhida (iniciando em 1).
     */
    static int solicitarEscolha(std::vector<std::string> opcoes);

    /**
     * @brief Mostra as barras de status (HP, MP, Energia) de um personagem.
     * * Utiliza polimorfismo para identificar se o personagem é um Aventureiro (exibindo recursos extras)
     * ou um Inimigo (exibindo apenas HP e Nível).
     * * @param p Referência constante para o personagem a ser exibido.
     */
    static void exibirStatus(const Personagem& p);

    /**
     * @brief Limpa o console para manter a interface organizada.
     * @note A implementação deve considerar compatibilidade entre Windows (cls) e Linux/Unix (clear).
     */
    static void limparTela();

    /**
     * @brief Exibe um efeito visual de separação para o combate.
     */
    static void renderizarDivisor();

    /**
     * @brief Pausa a execução e aguarda o jogador pressionar uma tecla para continuar.
     */
    static void pausar();
};

#endif