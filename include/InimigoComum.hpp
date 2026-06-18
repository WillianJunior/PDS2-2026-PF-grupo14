#ifndef INIMIGO_COMUM_HPP
#define INIMIGO_COMUM_HPP

#include "Inimigo.hpp"

/**
 * @class Goblin
 * @brief Um inimigo comum.
 */
class Goblin : public Inimigo {
public:
    // O parâmetro obrigatório 'nivel' vem antes do opcional 'nome'
    Goblin(std::string nome, int nivel);

    void executarTurno(Personagem& alvo) override;
};

/**
 * @class Slime
 * @brief Um inimigo básico.
 */
class Slime : public Inimigo {
public:
    // O parâmetro obrigatório 'nivel' vem antes do opcional 'nome'
    Slime( std::string nome, int nivel);

    void executarTurno(Personagem& alvo) override;
};

#endif // INIMIGO_COMUM_HPP