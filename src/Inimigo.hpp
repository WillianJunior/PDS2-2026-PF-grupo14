#ifndef INIMIGO_HPP
#define INIMIGO_HPP

#include "Personagem.hpp"

/**
 * @class Inimigo
 * @brief Entidade hostil que segue um padrão de ataque (script) pré-definido.
 */
class Inimigo : public Personagem {
public:
    /**
     * @param tipo String que define qual script será executado (ex: "Orc", "Slime").
     *//**
     * @brief Construtor da classe Inimigo.
     * @param nome Nome do inimigo.
     * @param hp Pontos de vida.
     * @param defesa Pontos de defesa.
     * @param nivel Nível do inimigo.
     * @param xp Experiência concedida ao derrotá-lo.
     * @param tipo Tipo do script (ex: "Orc").
     */
    Inimigo(std::string nome, int hp, int defesa, int nivel, int xp, std::string tipo);

    /** @brief Executa a ação do inimigo baseada em seu script e contador de turnos. */
    void executarTurno(Personagem& alvo) override;

    /** @brief Retorna a recompensa de XP ao morrer. */
    int getXPRecompensa() const;

private:
    std::string _tipoInimigo; 
    int _contadorTurnos;
    int _xpRecompensa;
};

#endif
