#ifndef INIMIGO_HPP
#define INIMIGO_HPP

#include "Personagem.hpp"
#include "Habilidade.hpp"
#include <vector>

/**
 * @class Inimigo
 * @brief Classe base para todas as entidades hostis do jogo.
 */
class Inimigo : public Personagem {
public:
    /** @brief Destrutor virtual para garantir a destruição correta das subclasses. */
    virtual ~Inimigo() = default;

    /** * @brief Executa o turno do inimigo. 
     * Pode ser sobrescrita pelas subclasses se cada tipo agir de forma única.
     */
    void executarTurno(Personagem& alvo) override;

    /** @brief Retorna a recompensa de XP ao morrer. */
    int getXPRecompensa() const;

    // Retorna apenas Nome, Nível e HP
    std::string getDeclaracaoStatus() const override;

protected:
   /**
     *//**
     * @brief Construtor da classe Inimigo.
     * @param nome Nome do inimigo.
     * @param hp Pontos de vida.
     * @param defesa Pontos de defesa.
     * @param nivel Nível do inimigo.
     * @param xp Experiência concedida ao derrotá-lo.
     */
    Inimigo(std::string nome, int hp, int defesa, int nivel, int xp);

    int _contadorTurnos;    ///< Contador de rodadas utilizado para gerenciar o padrão do monstro.   
    int _xpRecompensa;     ///< Experiência concedida ao jogador.
};

#endif