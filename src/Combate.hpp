#ifndef COMBATE_HPP
#define COMBATE_HPP

#include "Aventureiro.hpp"
#include "Inimigo.hpp"

/**
 * @class Combate
 * @brief Classe responsável por gerenciar o loop de batalha entre jogador e inimigo.
 */
class Combate {
public:
    Combate(Aventureiro& jogador, Inimigo& inimigo);

    /**
     * @brief Inicia a batalha.
     * @return true se o aventureiro venceu, false se perdeu.
     */
    bool iniciar();

private:
    Aventureiro& _jogador;
    Inimigo& _inimigo;
    int _turnoAtual;

    /** @brief Gerencia as entradas de menu do jogador durante a luta. */
    void turnoDoJogador();
    
    /** @brief Aciona o script do inimigo. */
    void turnoDoInimigo();
    
    /** @brief Verifica se ambos os combatentes ainda possuem HP. */
    bool verificarContinuacao();
};

#endif
