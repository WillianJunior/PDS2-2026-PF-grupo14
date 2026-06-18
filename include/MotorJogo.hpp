#ifndef MOTORJOGO_HPP
#define MOTORJOGO_HPP

#include "Aventureiro.hpp"
#include "Cena.hpp"
#include "InterfaceJogo.hpp"
#include <map>
#include <string>

// Forward declarations para evitar acoplamento desnecessário e inclusões circulares
class Inimigo; 
class Combate; 

/**
 * @file MotorJogo.hpp
 * @brief Definição da classe controladora do fluxo global do RPG (Engine).
 */

/**
 * @class MotorJogo
 * @brief Classe controladora (Engine) que gerencia o ciclo de vida do jogo, conectando a narrativa ao combate.
 * * O MotorJogo atua como a máquina de estados principal do RPG textual. Ele armazena o roteiro completo,
 * controla a transição de cenas com base nas escolhas do usuário, intercepta eventos para acionar
 * combates dinâmicos e manipula o estado do jogador em caso de progresso ou derrota.
 */
class MotorJogo {
public:
    /**
     * @brief Construtor da classe MotorJogo.
     * * Associa o herói principal ao motor e engatilha a inicialização da história.
     * * @param player Referência para o Aventureiro que passará pelos eventos do jogo.
     */
    MotorJogo(Aventureiro& player);

    /**
     * @brief Executa o loop principal de transição de cenas e eventos narrativos.
     * * Este método mantém o jogo ativo, renderizando os textos da cena atual (@c _idCenaAtual),
     * solicitando as escolhas do jogador através da interface e direcionando o fluxo para as 
     * próximas ramificações da história até atingir um estado final.
     */
    void rodar();

    /**
     * @brief Orquestra a criação do inimigo, o gerenciamento do combate e a limpeza de memória.
     * * Atua como uma fábrica (Factory) temporária. Com base na string fornecida, o método
     * instancia dinamicamente o inimigo correspondente (ex: um Goblin, Bruxa ou Dragão), 
     * cria o objeto @c Combate, inicia o loop de batalha e garante a desalocação correta dos ponteiros.
     * * @param tipoInimigo Identificador textual que define qual criatura será instanciada (ex: "Goblin", "Dragao").
     * @return true Se o jogador venceu o combate e pode prosseguir na narrativa.
     * @return false Se o jogador foi derrotado no confronto.
     * * @see Combate::iniciar()
     */
    bool realizarCombate(std::string tipoInimigo);

private:
    Aventureiro& _player;              ///< Referência ao herói controlado pelo jogador.
    InterfaceJogo _interface;         ///< Referencia à interface e suas funções.
    std::map<int, Cena> _roteiro;      ///< Dicionário que mapeia IDs únicos para suas respectivas Cenas.
    int _idCenaAtual;                  ///< ID indicador de qual cena está ativa no momento.

    /**
     * @brief Instancia e popula o mapa do roteiro com todas as cenas e escolhas possíveis.
     * * Método chamado internamente no construtor para carregar os textos, ramificações de 
     * diálogos e IDs de destinos estruturais da campanha.
     */
    void inicializarHistoria();
    
    /**
     * @brief Trata o fluxo de falha caso o jogador perca uma batalha.
     * * Interrompe a narrativa linear, limpa modificadores negativos e reconfigura o
     * @c _idCenaAtual para o último ponto de salvamento seguro (checkpoint) ou cena de recuperação.
     */
    void processarDerrota();
    
    /**
     * @brief Verifica e aplica bônus ou modificadores permanentes obtidos por escolhas narrativas.
     * * Analisa os metadados da escolha feita pelo jogador para identificar se ela engatilha
     * alterações de status, concessão de itens ou aprendizado de novas habilidades.
     * * @param escolha Referência constante para a estrutura de escolha que foi selecionada.
     */
    void checarEventosEspeciais(const Escolha& escolha);
};

#endif // MOTORJOGO_HPP