#ifndef COMBATE_HPP
#define COMBATE_HPP

#include "Aventureiro.hpp"
#include "Inimigo.hpp"

/**
 * @file Combate.hpp
 * @brief Gerenciador do ciclo de vida de um confronto no jogo.
 */

/**
 * @class Combate
 * @brief Classe responsável por mediar e controlar o loop de batalha por turnos.
 * * A classe Combate atua como um mediador (pattern Mediator) entre o jogador 
 * (Aventureiro) e o script (Inimigo). Ela dita a ordem das ações,
 * gerencia a renderização dos estados através da InterfaceJogo e determina as 
 * condições de vitória ou derrota, garantindo o baixo acoplamento entre as entidades combatentes.
 */
class Combate {
public:
    /**
     * @brief Construtor da classe Combate.
     * * Associa o jogador e o inimigo que se enfrentarão. Os combatentes são armazenados
     * por referência para que suas alterações de atributos (HP, Mana, Experiência) 
     * persistam após o término do confronto.
     * * @param jogador Referência para o Aventureiro controlado pelo usuário.
     * @param inimigo Referência para a subclasse de Inimigo que gerará o desafio.
     * @pre Ambas as entidades devem estar vivas (@c estaVivo() == true) para iniciar o combate.
     */
    Combate(Aventureiro& jogador, Inimigo& inimigo);

    /**
     * @brief Inicia o loop principal da batalha.
     * * Executa turnos alternados entre o jogador e o inimigo até que uma das entidades
     * seja derrotada. Ao final, processa as recompensas ou a tela de game over.
     * * @return true Se o aventureiro venceu o combate.
     * @return false Se o aventureiro foi derrotado.
     * * @note Este método bloqueia a execução principal até que o combate seja resolvido.
     */
    bool iniciar();

private:
    Aventureiro& _jogador; ///< Referência ao herói do jogador.
    Inimigo& _inimigo;     ///< Referência ao monstro/adversário atual.
    int _turnoAtual;       ///< Contador incremental de turnos da batalha.

    /**
     * @brief Processa a fase de tomada de decisão do jogador.
     * * Solicita as opções de ação via InterfaceJogo (Atacar, Habilidades, Itens) e
     * aplica os efeitos escolhidos contra o alvo (@c _inimigo).
     */
    void turnoDoJogador();
    
    /**
     * @brief Dispara o script de ação da inteligência artificial do inimigo.
     * * Invoca o método polimórfico @c executarTurno do inimigo atual, passando
     * o aventureiro como alvo da ação hostil.
     */
    void turnoDoInimigo();
    
    /**
     * @brief Valida o estado de saúde dos combatentes para continuar a luta.
     * @return true Se ambos ainda possuírem HP acima de zero.
     * @return false Se pelo menos um dos combatentes morreu.
     */
    bool verificarContinuacao();

    /**
     * @brief Limpa o console e renderiza o cabeçalho do turno atual.
     * * Desenha as barras de status atualizadas do jogador e do inimigo usando
     * os métodos utilitários da InterfaceJogo.
     */
    void renderizarInterfaceTurno();

    /**
     * @brief Exibe a introdução visual e narrativa do confronto.
     * * Mostra mensagens de aviso informando quais entidades entraram em rota de colisão.
     */
    void renderizarInicioCombate();

    /**
     * @brief Renderiza a tela de desfecho do combate.
     * * Exibe mensagens de congratulações e a experiência ganha em caso de vitória,
     * ou uma tela fúnebre em caso de derrota.
     * * @param jogadorVenceu Flag indicando o resultado final da apuração do combate.
     */
    void renderizarFimCombate(bool jogadorVenceu);
};

#endif