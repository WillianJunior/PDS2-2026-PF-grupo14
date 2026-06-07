#ifndef INIMIGOS_INCOMUNS_HPP
#define INIMIGOS_INCOMUNS_HPP

#include "Inimigo.hpp"

//Esses inimigos são apenas exemplos por enquanto.

class Bruxa : public Inimigo {
public:
   
    Bruxa(std::string nome, int nivel);

    void executarTurno(Personagem& alvo) override;

private:
    int _mana; ///< Atributo exclusivo para controlar o uso de feitiços.
};

class GolemPedra : public Inimigo {
public:

    GolemPedra(std::string nome, int nivel);

    void executarTurno(Personagem& alvo) override;

    void receberDano(int dano) {};
};

#endif // INIMIGOS_INCOMUNS_HPP