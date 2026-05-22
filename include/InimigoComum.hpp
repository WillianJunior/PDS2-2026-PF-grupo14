#ifndef INIMIGO_COMUM.HPP
#define INIMIGO_COMUM.HPP
#include "Inimigo.hpp"

//Esses inimigos são apenas exemplos por enquanto.

/**
 * @class Goblin
 * @brief Um inimigo comum.
 */
class Goblin : public Inimigo {
public:
    /** @brief Construtor que inicializa um Goblin padrão com base no nível. */
    Goblin(std::string nome = "Goblin", int nivel);

    /** @brief Sobrescreve o turno para dar o comportamento ágil do Goblin. */
    void executarTurno(Personagem& alvo) override;
};

/**
 * @class Slime
 * @brief Um inimigo básico.
 */
class Slime : public Inimigo {
public:
    Slime(std::string nome = "Slime Azul", int nivel);

    void executarTurno(Personagem& alvo) override;
};







#endif